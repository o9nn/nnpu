#pragma once
/**
 * @file types.hpp
 * @brief Core type definitions for Modern OpenCog
 *
 * Design principles:
 * - Cache-line aware memory layout (64 bytes)
 * - Data-oriented design (Structure of Arrays)
 * - Lock-free where possible
 * - SIMD-friendly data alignment
 */

#include <cstdint>
#include <atomic>
#include <bit>
#include <compare>
#include <concepts>
#include <limits>
#include <span>
#include <string_view>

namespace opencog {

// ============================================================================
// Fundamental Types
// ============================================================================

/// Unique atom identifier - 48 bits usable, 16 bits for type
struct AtomId {
    uint64_t value{0};

    constexpr AtomId() = default;
    constexpr explicit AtomId(uint64_t v) : value(v) {}

    [[nodiscard]] constexpr bool valid() const noexcept { return value != 0; }
    [[nodiscard]] constexpr explicit operator bool() const noexcept { return valid(); }

    constexpr auto operator<=>(const AtomId&) const = default;

    // Extract components
    [[nodiscard]] constexpr uint64_t index() const noexcept { return value & 0x0000FFFFFFFFFFFF; }
    [[nodiscard]] constexpr uint16_t generation() const noexcept { return static_cast<uint16_t>(value >> 48); }

    // Create from components (for slot reuse with generation tracking)
    [[nodiscard]] static constexpr AtomId make(uint64_t idx, uint16_t gen) noexcept {
        return AtomId{(static_cast<uint64_t>(gen) << 48) | (idx & 0x0000FFFFFFFFFFFF)};
    }
};

static_assert(sizeof(AtomId) == 8);

/// Null atom constant
inline constexpr AtomId ATOM_NULL{0};

// ============================================================================
// Atom Types - Extensible via enum
// ============================================================================

enum class AtomType : uint16_t {
    // Special
    INVALID = 0,

    // Nodes (1-999)
    NODE = 1,
    CONCEPT_NODE = 2,
    PREDICATE_NODE = 3,
    VARIABLE_NODE = 4,
    NUMBER_NODE = 5,
    TYPE_NODE = 6,
    GROUNDED_OBJECT_NODE = 7,
    ANCHOR_NODE = 8,
    SCHEMA_NODE = 9,
    GROUNDED_SCHEMA_NODE = 10,
    DEFINED_SCHEMA_NODE = 11,

    // Links (1000+)
    LINK = 1000,
    ORDERED_LINK = 1001,
    UNORDERED_LINK = 1002,

    // Logical Links
    AND_LINK = 1010,
    OR_LINK = 1011,
    NOT_LINK = 1012,

    // Inheritance/Similarity
    INHERITANCE_LINK = 1020,
    SIMILARITY_LINK = 1021,
    SUBSET_LINK = 1022,
    INTENSIONAL_INHERITANCE_LINK = 1023,

    // Evaluation
    EVALUATION_LINK = 1030,
    EXECUTION_LINK = 1031,
    EXECUTION_OUTPUT_LINK = 1032,

    // Variables and Scoping
    SCOPE_LINK = 1040,
    BIND_LINK = 1041,
    GET_LINK = 1042,
    PUT_LINK = 1043,
    LAMBDA_LINK = 1044,

    // Context/State
    CONTEXT_LINK = 1050,
    STATE_LINK = 1051,
    DEFINE_LINK = 1052,

    // Temporal
    AT_TIME_LINK = 1060,
    TIME_INTERVAL_LINK = 1061,
    BEFORE_LINK = 1062,
    OVERLAPS_LINK = 1063,

    // PLN-specific
    IMPLICATION_LINK = 1100,
    EQUIVALENCE_LINK = 1101,
    FORALL_LINK = 1102,
    EXISTS_LINK = 1103,
    AVERAGE_LINK = 1104,

    // User-defined types start here
    USER_DEFINED = 10000
};

[[nodiscard]] constexpr bool is_node(AtomType t) noexcept {
    return static_cast<uint16_t>(t) > 0 && static_cast<uint16_t>(t) < 1000;
}

[[nodiscard]] constexpr bool is_link(AtomType t) noexcept {
    return static_cast<uint16_t>(t) >= 1000;
}

// ============================================================================
// Truth Value - Compact representation (8 bytes)
// ============================================================================

/**
 * @brief Probabilistic truth value using half-precision floats
 *
 * Uses 16-bit floats for strength and confidence to fit in 8 bytes total.
 * For operations requiring precision, convert to float32.
 */
struct alignas(8) TruthValue {
    // Using float for now, can switch to _Float16 on supporting compilers
    float strength{0.0f};      ///< Probability/certainty [0,1]
    float confidence{0.0f};    ///< Meta-certainty about strength [0,1]

    constexpr TruthValue() = default;
    constexpr TruthValue(float s, float c) : strength(s), confidence(c) {}

    // Named constructors
    [[nodiscard]] static constexpr TruthValue simple(float s) noexcept {
        return TruthValue{s, 0.9f};
    }

    [[nodiscard]] static constexpr TruthValue default_tv() noexcept {
        return TruthValue{1.0f, 0.0f};
    }

    [[nodiscard]] static constexpr TruthValue true_tv() noexcept {
        return TruthValue{1.0f, 0.9f};
    }

    [[nodiscard]] static constexpr TruthValue false_tv() noexcept {
        return TruthValue{0.0f, 0.9f};
    }

    // Semantic queries
    [[nodiscard]] constexpr bool is_true(float threshold = 0.5f) const noexcept {
        return strength >= threshold;
    }

    [[nodiscard]] constexpr bool is_confident(float threshold = 0.5f) const noexcept {
        return confidence >= threshold;
    }

    // Count representation (for PLN formulas)
    [[nodiscard]] float count() const noexcept {
        // K is a scaling constant, typically 800
        constexpr float K = 800.0f;
        return confidence * K / (1.0f - confidence + 1e-10f);
    }

    [[nodiscard]] static TruthValue from_count(float s, float n) noexcept {
        constexpr float K = 800.0f;
        return TruthValue{s, n / (n + K)};
    }

    constexpr auto operator<=>(const TruthValue&) const = default;
};

static_assert(sizeof(TruthValue) == 8);

// ============================================================================
// Attention Value - Compact representation (8 bytes)
// ============================================================================

/**
 * @brief Economic Attention Network values
 *
 * STI: Short-Term Importance - current focus
 * LTI: Long-Term Importance - sustained relevance
 * VLTI: Very Long-Term Importance flag - prevents forgetting
 */
struct alignas(8) AttentionValue {
    float sti{0.0f};           ///< Short-term importance [-1, 1] normalized
    int16_t lti{0};            ///< Long-term importance (integer for atomics)
    uint8_t vlti{0};           ///< Very long-term importance flag
    uint8_t _pad{0};           ///< Padding for alignment

    constexpr AttentionValue() = default;
    constexpr AttentionValue(float s, int16_t l, uint8_t v = 0)
        : sti(s), lti(l), vlti(v) {}

    [[nodiscard]] static constexpr AttentionValue default_av() noexcept {
        return AttentionValue{0.0f, 0, 0};
    }

    [[nodiscard]] static constexpr AttentionValue disposable() noexcept {
        return AttentionValue{-1.0f, 0, 0};
    }

    [[nodiscard]] constexpr bool is_disposable() const noexcept {
        return vlti == 0 && sti < -0.9f;
    }

    [[nodiscard]] constexpr bool in_attentional_focus(float threshold = 0.0f) const noexcept {
        return sti >= threshold;
    }
};

static_assert(sizeof(AttentionValue) == 8);

// ============================================================================
// Atom Header - Minimal inline metadata (16 bytes)
// ============================================================================

/**
 * @brief Compact atom header stored inline in atom table
 *
 * Contains only the most frequently accessed data.
 * Extended data (name, outgoing set) stored separately.
 */
struct alignas(16) AtomHeader {
    AtomType type{AtomType::INVALID};
    uint16_t flags{0};
    uint32_t incoming_count{0};  // Number of links pointing to this atom
    uint64_t hash{0};            // Precomputed hash for fast lookup

    // Flag bits
    static constexpr uint16_t FLAG_CHECKED = 1 << 0;
    static constexpr uint16_t FLAG_MARKED = 1 << 1;
    static constexpr uint16_t FLAG_EXTERNAL = 1 << 2;

    [[nodiscard]] constexpr bool has_flag(uint16_t f) const noexcept {
        return (flags & f) != 0;
    }

    constexpr void set_flag(uint16_t f) noexcept { flags |= f; }
    constexpr void clear_flag(uint16_t f) noexcept { flags &= ~f; }
};

static_assert(sizeof(AtomHeader) == 16);

// ============================================================================
// Concepts for generic programming
// ============================================================================

template<typename T>
concept AtomLike = requires(T t) {
    { t.id() } -> std::convertible_to<AtomId>;
    { t.type() } -> std::convertible_to<AtomType>;
};

template<typename T>
concept TruthValued = requires(T t) {
    { t.truth_value() } -> std::convertible_to<TruthValue>;
};

template<typename T>
concept AttentionValued = requires(T t) {
    { t.attention_value() } -> std::convertible_to<AttentionValue>;
};

template<typename T>
concept HasOutgoing = requires(T t) {
    { t.outgoing() } -> std::convertible_to<std::span<const AtomId>>;
    { t.arity() } -> std::convertible_to<size_t>;
};

// ============================================================================
// Handle - Reference-counted atom pointer (for external use)
// ============================================================================

class AtomSpace;  // Forward declaration

/**
 * @brief Safe handle to an atom in an AtomSpace
 *
 * Handles are lightweight (16 bytes) and can be freely copied.
 * They maintain validity through generation checking.
 */
class Handle {
    AtomId id_{};
    AtomSpace* space_{nullptr};

public:
    constexpr Handle() = default;
    constexpr Handle(AtomId id, AtomSpace* space) : id_(id), space_(space) {}

    [[nodiscard]] constexpr AtomId id() const noexcept { return id_; }
    [[nodiscard]] constexpr AtomSpace* space() const noexcept { return space_; }
    [[nodiscard]] constexpr bool valid() const noexcept { return id_.valid() && space_; }
    [[nodiscard]] constexpr explicit operator bool() const noexcept { return valid(); }

    constexpr auto operator<=>(const Handle&) const = default;
};

static_assert(sizeof(Handle) == 16);

// ============================================================================
// Utility functions
// ============================================================================

/// Fast hash combining (like boost::hash_combine)
[[nodiscard]] constexpr uint64_t hash_combine(uint64_t seed, uint64_t value) noexcept {
    return seed ^ (value + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2));
}

/// Type name lookup (defined in types.cpp)
[[nodiscard]] std::string_view type_name(AtomType type) noexcept;

/// Parse type from name
[[nodiscard]] AtomType type_from_name(std::string_view name) noexcept;

} // namespace opencog

// Standard library specializations
template<>
struct std::hash<opencog::AtomId> {
    [[nodiscard]] size_t operator()(const opencog::AtomId& id) const noexcept {
        return std::hash<uint64_t>{}(id.value);
    }
};

template<>
struct std::hash<opencog::Handle> {
    [[nodiscard]] size_t operator()(const opencog::Handle& h) const noexcept {
        return std::hash<uint64_t>{}(h.id().value);
    }
};
