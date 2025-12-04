/**
 * @file types.cpp
 * @brief Core type implementations
 */

#include <opencog/core/types.hpp>

#include <unordered_map>

namespace opencog {

namespace {

// Type name registry
const std::unordered_map<AtomType, std::string_view> TYPE_NAMES = {
    // Special
    {AtomType::INVALID, "Invalid"},

    // Nodes
    {AtomType::NODE, "Node"},
    {AtomType::CONCEPT_NODE, "ConceptNode"},
    {AtomType::PREDICATE_NODE, "PredicateNode"},
    {AtomType::VARIABLE_NODE, "VariableNode"},
    {AtomType::NUMBER_NODE, "NumberNode"},
    {AtomType::TYPE_NODE, "TypeNode"},
    {AtomType::GROUNDED_OBJECT_NODE, "GroundedObjectNode"},
    {AtomType::ANCHOR_NODE, "AnchorNode"},
    {AtomType::SCHEMA_NODE, "SchemaNode"},
    {AtomType::GROUNDED_SCHEMA_NODE, "GroundedSchemaNode"},
    {AtomType::DEFINED_SCHEMA_NODE, "DefinedSchemaNode"},

    // Links
    {AtomType::LINK, "Link"},
    {AtomType::ORDERED_LINK, "OrderedLink"},
    {AtomType::UNORDERED_LINK, "UnorderedLink"},

    // Logical
    {AtomType::AND_LINK, "AndLink"},
    {AtomType::OR_LINK, "OrLink"},
    {AtomType::NOT_LINK, "NotLink"},

    // Inheritance
    {AtomType::INHERITANCE_LINK, "InheritanceLink"},
    {AtomType::SIMILARITY_LINK, "SimilarityLink"},
    {AtomType::SUBSET_LINK, "SubsetLink"},
    {AtomType::INTENSIONAL_INHERITANCE_LINK, "IntensionalInheritanceLink"},

    // Evaluation
    {AtomType::EVALUATION_LINK, "EvaluationLink"},
    {AtomType::EXECUTION_LINK, "ExecutionLink"},
    {AtomType::EXECUTION_OUTPUT_LINK, "ExecutionOutputLink"},

    // Scoping
    {AtomType::SCOPE_LINK, "ScopeLink"},
    {AtomType::BIND_LINK, "BindLink"},
    {AtomType::GET_LINK, "GetLink"},
    {AtomType::PUT_LINK, "PutLink"},
    {AtomType::LAMBDA_LINK, "LambdaLink"},

    // Context
    {AtomType::CONTEXT_LINK, "ContextLink"},
    {AtomType::STATE_LINK, "StateLink"},
    {AtomType::DEFINE_LINK, "DefineLink"},

    // Temporal
    {AtomType::AT_TIME_LINK, "AtTimeLink"},
    {AtomType::TIME_INTERVAL_LINK, "TimeIntervalLink"},
    {AtomType::BEFORE_LINK, "BeforeLink"},
    {AtomType::OVERLAPS_LINK, "OverlapsLink"},

    // PLN
    {AtomType::IMPLICATION_LINK, "ImplicationLink"},
    {AtomType::EQUIVALENCE_LINK, "EquivalenceLink"},
    {AtomType::FORALL_LINK, "ForAllLink"},
    {AtomType::EXISTS_LINK, "ExistsLink"},
    {AtomType::AVERAGE_LINK, "AverageLink"},
};

// Reverse lookup map (built lazily)
std::unordered_map<std::string_view, AtomType> NAME_TO_TYPE;

void ensure_reverse_map() {
    if (NAME_TO_TYPE.empty()) {
        for (const auto& [type, name] : TYPE_NAMES) {
            NAME_TO_TYPE[name] = type;
        }
    }
}

} // anonymous namespace

std::string_view type_name(AtomType type) noexcept {
    auto it = TYPE_NAMES.find(type);
    if (it != TYPE_NAMES.end()) {
        return it->second;
    }

    // Check if it's a user-defined type
    if (static_cast<uint16_t>(type) >= static_cast<uint16_t>(AtomType::USER_DEFINED)) {
        return "UserDefinedType";
    }

    return "UnknownType";
}

AtomType type_from_name(std::string_view name) noexcept {
    ensure_reverse_map();

    auto it = NAME_TO_TYPE.find(name);
    if (it != NAME_TO_TYPE.end()) {
        return it->second;
    }

    return AtomType::INVALID;
}

} // namespace opencog
