#pragma once

#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <cstdint>

namespace ggnucash {
namespace vdev {

// Forward declaration
class LlamaCoprocessorDriver;

// ============================================================================
// Entelechy: Vital Actualization Framework for NPU
// ============================================================================
// Implements the five-dimensional assessment framework that transforms NPU
// from static hardware into a living, self-actualizing cognitive coprocessor.
//
// Five Dimensions:
//   1. Ontological (BEING) - structural integrity and completeness
//   2. Teleological (PURPOSE) - drive toward actualization
//   3. Cognitive (COGNITION) - reasoning and learning capabilities
//   4. Integrative (INTEGRATION) - coherence of the whole
//   5. Evolutionary (GROWTH) - capacity for self-improvement
// ============================================================================

// ----------------------------------------------------------------------------
// Dimension 1: Ontological (BEING)
// ----------------------------------------------------------------------------

struct OntologicalHealth {
    double foundation_integrity;      // VirtualPCB, memory, controllers (0.0-1.0)
    double core_completeness;         // Driver, GGUF runtime (0.0-1.0)
    double specialized_features;      // Token streaming, KV-cache, etc (0.0-1.0)
    double architectural_coherence;   // Overall structural health (0.0-1.0)
    
    std::vector<std::string> components_present;
    std::vector<std::string> components_missing;
    int total_components;
    int healthy_components;
};

// ----------------------------------------------------------------------------
// Dimension 2: Teleological (PURPOSE)
// ----------------------------------------------------------------------------

enum class DevelopmentPhase {
    PHASE_1_FOUNDATION = 1,      // Virtual device infrastructure
    PHASE_2_CORE_INTEGRATION,    // GGUF runtime integration
    PHASE_3_PRODUCTION_FEATURES, // KV-cache, GPU, batch, etc
    PHASE_4_ENTELECHY_ONTOGENESIS, // Self-actualization framework
    PHASE_5_SELF_TRANSCENDENCE   // Autonomous self-improvement
};

struct PhaseProgress {
    DevelopmentPhase phase;
    std::string name;
    double completion;            // 0.0-1.0
    std::vector<std::string> completed_items;
    std::vector<std::string> pending_items;
};

struct TeleologicalAlignment {
    std::map<DevelopmentPhase, PhaseProgress> phases;
    double roadmap_alignment;         // 0.0-1.0
    double actualization_trajectory;  // Growth vector (0.0-1.0)
    double purpose_clarity;           // Goal definition clarity (0.0-1.0)
};

// ----------------------------------------------------------------------------
// Dimension 3: Cognitive (COGNITION)
// ----------------------------------------------------------------------------

struct CognitiveCompleteness {
    double inference_quality;         // GGUF execution quality (0.0-1.0)
    double performance_intelligence;  // Telemetry effectiveness (0.0-1.0)
    double meta_cognitive_depth;      // Self-awareness level (0.0-1.0)
    double overall_cognition;         // Combined cognitive health (0.0-1.0)
    
    // Performance metrics
    double avg_tokens_per_second;
    uint64_t total_inferences;
    double error_rate;
};

// ----------------------------------------------------------------------------
// Dimension 4: Integrative (INTEGRATION)
// ----------------------------------------------------------------------------

struct IntegrativeHealth {
    double hardware_integration;      // VirtualPCB attachment (0.0-1.0)
    double software_coherence;        // API consistency (0.0-1.0)
    double system_unity;              // Device coexistence (0.0-1.0)
    double overall_integration;       // Holistic integration (0.0-1.0)
    
    int total_integration_points;
    int healthy_integration_points;
};

// ----------------------------------------------------------------------------
// Dimension 5: Evolutionary (GROWTH)
// ----------------------------------------------------------------------------

struct EvolutionaryPotential {
    int todo_count;                   // Remaining work items
    int fixme_count;                  // Issues to resolve
    int stub_count;                   // Stubbed implementations
    double implementation_depth;      // How "real" vs "stubbed" (0.0-1.0)
    double self_improvement_capacity; // Potential for growth (0.0-1.0)
    double evolutionary_fitness;      // Overall growth potential (0.0-1.0)
    
    std::vector<std::string> improvement_areas;
};

// ----------------------------------------------------------------------------
// Complete Entelechy Assessment
// ----------------------------------------------------------------------------

struct EntelechyAssessment {
    // Five dimensions
    OntologicalHealth ontological;
    TeleologicalAlignment teleological;
    CognitiveCompleteness cognitive;
    IntegrativeHealth integrative;
    EvolutionaryPotential evolutionary;
    
    // Overall metrics
    double overall_actualization;     // 0.0-1.0 (weighted average)
    double fitness_score;             // Combined fitness
    
    // Development stage
    enum Stage {
        EMBRYONIC,      // < 30% actualization
        JUVENILE,       // 30-60% actualization
        MATURE,         // 60-80% actualization
        TRANSCENDENT    // > 80% actualization
    } current_stage;
    
    // Recommendations
    std::vector<std::string> improvement_recommendations;
    std::vector<std::string> critical_issues;
    
    // Timestamp
    std::chrono::steady_clock::time_point assessment_time;
};

// ----------------------------------------------------------------------------
// Entelechy Introspector
// ----------------------------------------------------------------------------

class NPUEntelechyIntrospector {
public:
    // Assess single NPU instance
    static EntelechyAssessment assess(const LlamaCoprocessorDriver* npu);
    
    // Assess each dimension individually
    static OntologicalHealth assessOntological(const LlamaCoprocessorDriver* npu);
    static TeleologicalAlignment assessTeleological(const LlamaCoprocessorDriver* npu);
    static CognitiveCompleteness assessCognitive(const LlamaCoprocessorDriver* npu);
    static IntegrativeHealth assessIntegrative(const LlamaCoprocessorDriver* npu);
    static EvolutionaryPotential assessEvolutionary(const LlamaCoprocessorDriver* npu);
    
    // Calculate overall metrics
    static double calculateActualization(const EntelechyAssessment& assessment);
    static double calculateFitness(const EntelechyAssessment& assessment);
    static EntelechyAssessment::Stage determineStage(double actualization);
    
    // Generate improvement recommendations
    static std::vector<std::string> generateRecommendations(const EntelechyAssessment& assessment);
    
    // Pretty-print assessment
    static std::string formatAssessment(const EntelechyAssessment& assessment);
    static std::string formatDimensionReport(const std::string& dimension_name, double health);
};

// ----------------------------------------------------------------------------
// Fitness Weights
// ----------------------------------------------------------------------------

struct EntelechyWeights {
    double ontological = 0.20;    // Structural foundation
    double teleological = 0.25;   // Purpose clarity & progress
    double cognitive = 0.25;      // Reasoning capability
    double integrative = 0.15;    // Component coherence
    double evolutionary = 0.15;   // Growth capacity
};

} // namespace vdev
} // namespace ggnucash
