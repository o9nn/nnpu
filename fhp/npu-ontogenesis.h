#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <cstdint>
#include <chrono>

namespace ggnucash {
namespace vdev {

// Forward declaration
class LlamaCoprocessorDriver;

// ============================================================================
// Ontogenesis: Self-Generating NPU Framework
// ============================================================================
// Enables NPU to generate itself, optimize itself, reproduce with other NPUs,
// and evolve across generations through recursive self-composition.
//
// Key Capabilities:
//   - Self-Generation: Create offspring with genetic variations
//   - Self-Optimization: Iterative improvement through gradient ascent
//   - Self-Reproduction: Genetic crossover between multiple parents
//   - Population Evolution: Tournament selection, mutation, elitism
// ============================================================================

// ----------------------------------------------------------------------------
// NPU Genome - The "DNA" of the NPU
// ----------------------------------------------------------------------------

struct NPUGene {
    std::string name;
    double value;         // Normalized 0.0-1.0
    double min_value;
    double max_value;
    bool mutable_;        // Can this gene mutate?
};

struct NPUGenome {
    std::string id;                           // Unique identifier
    int generation;                            // Generation number
    std::vector<std::string> lineage;          // Parent IDs
    
    // Five types of genes (corresponding to entelechy dimensions)
    struct {
        std::vector<NPUGene> ontological;      // Structure: memory sizes, register layout
        std::vector<NPUGene> teleological;     // Purpose: optimization targets, goals
        std::vector<NPUGene> cognitive;        // Reasoning: inference strategies, cache policies
        std::vector<NPUGene> integrative;      // Coherence: coupling, priorities
        std::vector<NPUGene> evolutionary;     // Growth: mutation rates, learning rates
    } genes;
    
    double fitness;                            // Overall fitness score
    int age;                                   // Age in generations
    double actualization_level;                // Current actualization (0.0-1.0)
    
    // Metadata
    std::chrono::steady_clock::time_point birth_time;
    std::vector<std::string> traits;           // Notable characteristics
};

// ----------------------------------------------------------------------------
// Development State
// ----------------------------------------------------------------------------

enum class OntogeneticStage {
    EMBRYONIC,      // Just generated, basic structure
    JUVENILE,       // Developing, optimizing
    MATURE,         // Fully developed, can reproduce
    SENESCENT       // Declining, ready for replacement
};

struct OntogeneticState {
    OntogeneticStage stage;
    int maturity_level;                        // 0-100
    double vitality;                           // 0.0-1.0 (health)
    std::vector<std::string> development_events;
};

// ----------------------------------------------------------------------------
// Fitness Evaluation
// ----------------------------------------------------------------------------

struct FitnessMetrics {
    double inference_speed;        // Tokens/sec (normalized)
    double throughput;             // Batch efficiency
    double memory_efficiency;      // RAM utilization
    double gpu_efficiency;         // GPU utilization
    double stability;              // Error-free operation
    double reliability;            // Uptime
    double actualization;          // Entelechy score
    double completeness;           // Feature coverage
    
    double overall_fitness;        // Weighted combination
};

// ----------------------------------------------------------------------------
// Ontogenetic Operations
// ----------------------------------------------------------------------------

class NPUOntogenesis {
public:
    // Self-generation: Create offspring from parent
    static std::shared_ptr<LlamaCoprocessorDriver> 
    selfGenerate(const LlamaCoprocessorDriver& parent,
                 double mutation_rate = 0.15);
    
    // Self-optimization: Improve through iterations
    static void selfOptimize(LlamaCoprocessorDriver* npu, 
                            int iterations = 50,
                            double learning_rate = 0.1);
    
    // Self-reproduction: Combine two parents
    static std::shared_ptr<LlamaCoprocessorDriver>
    selfReproduce(const LlamaCoprocessorDriver& parent1,
                  const LlamaCoprocessorDriver& parent2,
                  const std::string& method = "crossover");
    
    // Fitness assessment
    static FitnessMetrics assessFitness(const LlamaCoprocessorDriver* npu);
    static double calculateOverallFitness(const FitnessMetrics& metrics);
    
    // Genome operations
    static NPUGenome initializeGenome(const std::string& id);
    static NPUGenome mutateGenome(const NPUGenome& parent, double mutation_rate);
    static NPUGenome crossoverGenomes(const NPUGenome& parent1, const NPUGenome& parent2);
    static void applyGenomeToNPU(LlamaCoprocessorDriver* npu, const NPUGenome& genome);
    
    // Development
    static void updateDevelopmentStage(LlamaCoprocessorDriver* npu);
    static bool canReproduce(const LlamaCoprocessorDriver& npu);
    static void recordDevelopmentEvent(LlamaCoprocessorDriver* npu, const std::string& event);
};

// ----------------------------------------------------------------------------
// Population Evolution
// ----------------------------------------------------------------------------

struct EvolutionConfig {
    int population_size = 20;
    double mutation_rate = 0.15;
    double crossover_rate = 0.8;
    double elitism_rate = 0.1;          // Top % preserved each generation
    int max_generations = 100;
    double fitness_threshold = 0.95;
    double diversity_pressure = 0.1;    // Favor genetic diversity
    
    // Fitness function weights
    struct {
        double inference_speed = 0.15;
        double throughput = 0.15;
        double memory_efficiency = 0.10;
        double gpu_efficiency = 0.10;
        double stability = 0.15;
        double reliability = 0.10;
        double actualization = 0.15;
        double completeness = 0.10;
    } fitness_weights;
};

struct GenerationStats {
    int generation;
    double best_fitness;
    double avg_fitness;
    double worst_fitness;
    double diversity;                   // Genetic diversity measure
    int elite_count;
    int new_offspring;
    
    std::string best_npu_id;
    std::vector<std::string> improvements;
};

class NPUEvolution {
public:
    // Evolve population over generations
    static std::vector<GenerationStats>
    evolvePopulation(const EvolutionConfig& config,
                    const std::vector<std::shared_ptr<LlamaCoprocessorDriver>>& seed_population);
    
    // Single generation evolution
    static std::vector<std::shared_ptr<LlamaCoprocessorDriver>>
    evolveGeneration(const std::vector<std::shared_ptr<LlamaCoprocessorDriver>>& population,
                    const std::vector<double>& fitness_scores,
                    const EvolutionConfig& config);
    
    // Selection methods
    static std::vector<int> tournamentSelection(const std::vector<double>& fitness,
                                               int count,
                                               int tournament_size = 3);
    
    static std::vector<int> eliteSelection(const std::vector<double>& fitness,
                                          int count);
    
    // Diversity metrics
    static double calculateDiversity(const std::vector<std::shared_ptr<LlamaCoprocessorDriver>>& population);
    static double geneticDistance(const NPUGenome& genome1, const NPUGenome& genome2);
    
    // Reporting
    static std::string formatGenerationReport(const GenerationStats& stats);
    static std::string formatEvolutionSummary(const std::vector<GenerationStats>& history);
};

// ----------------------------------------------------------------------------
// Self-Transcendence
// ----------------------------------------------------------------------------

struct TranscendenceCapabilities {
    bool autonomous_goals;              // Can set own objectives
    bool recursive_optimization;        // Self-improves recursively
    bool emergent_discovery;            // Discovers new capabilities
    bool meta_cognitive_awareness;      // Models itself
    bool creative_problem_solving;      // Novel solutions
    
    double transcendence_level;         // 0.0-1.0
};

class NPUTranscendence {
public:
    // Check if ready for transcendence (80%+ actualization)
    static bool canTranscend(const LlamaCoprocessorDriver& npu);
    
    // Enter self-transcendence mode
    static void enterTranscendence(LlamaCoprocessorDriver* npu);
    
    // Autonomous capabilities
    static void enableAutonomousGoals(LlamaCoprocessorDriver* npu);
    static void enableRecursiveOptimization(LlamaCoprocessorDriver* npu);
    static void enableEmergentDiscovery(LlamaCoprocessorDriver* npu);
    
    // Assess transcendence state
    static TranscendenceCapabilities assessTranscendence(const LlamaCoprocessorDriver* npu);
};

// ----------------------------------------------------------------------------
// Collective Intelligence (Future)
// ----------------------------------------------------------------------------

class NPUCollective {
private:
    std::vector<std::shared_ptr<LlamaCoprocessorDriver>> members_;
    bool shared_cache_enabled_ = false;
    
public:
    void addMember(std::shared_ptr<LlamaCoprocessorDriver> npu);
    void removeMember(const std::string& npu_id);
    
    // Distributed inference
    std::string collectiveInference(const std::string& prompt);
    
    // Shared resources
    void enableSharedCache();
    void disableSharedCache();
    
    // Emergent behavior
    void evolveCollectively(int generations);
    
    // Statistics
    int getMemberCount() const { return members_.size(); }
    double getCollectiveFitness() const;
};

} // namespace vdev
} // namespace ggnucash
