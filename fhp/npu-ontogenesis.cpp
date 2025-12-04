#include "npu-ontogenesis.h"
#include "llama-coprocessor-driver.h"
#include "npu-entelechy.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <cmath>

namespace ggnucash {
namespace vdev {

// ============================================================================
// Helper function to access genome safely
// ============================================================================

static NPUGenome& getGenome(LlamaCoprocessorDriver* npu) {
    if (!npu->genome_) {
        npu->genome_ = new NPUGenome(NPUOntogenesis::initializeGenome("NPU-" + std::to_string(rand())));
    }
    return *npu->genome_;
}

static const NPUGenome& getGenome(const LlamaCoprocessorDriver& npu) {
    if (!npu.genome_) {
        // Const context - shouldn't modify, but need a default
        static NPUGenome default_genome = NPUOntogenesis::initializeGenome("NPU-default");
        return default_genome;
    }
    return *npu.genome_;
}

static OntogeneticState& getState(LlamaCoprocessorDriver* npu) {
    if (!npu->ontogenetic_state_) {
        npu->ontogenetic_state_ = new OntogeneticState();
        npu->ontogenetic_state_->stage = OntogeneticStage::EMBRYONIC;
        npu->ontogenetic_state_->maturity_level = 0;
        npu->ontogenetic_state_->vitality = 1.0;
    }
    return *npu->ontogenetic_state_;
}

static TranscendenceCapabilities& getTranscendence(LlamaCoprocessorDriver* npu) {
    if (!npu->transcendence_) {
        npu->transcendence_ = new TranscendenceCapabilities();
        npu->transcendence_->autonomous_goals = false;
        npu->transcendence_->recursive_optimization = false;
        npu->transcendence_->emergent_discovery = false;
        npu->transcendence_->meta_cognitive_awareness = false;
        npu->transcendence_->creative_problem_solving = false;
        npu->transcendence_->transcendence_level = 0.0;
    }
    return *npu->transcendence_;
}

// ============================================================================
// Random number generation
// ============================================================================

static std::random_device rd;
static std::mt19937 gen(rd());

static double randomDouble(double min = 0.0, double max = 1.0) {
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

static int randomInt(int min, int max) {
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

// ============================================================================
// Genome Initialization
// ============================================================================

NPUGenome NPUOntogenesis::initializeGenome(const std::string& id) {
    NPUGenome genome;
    genome.id = id;
    genome.generation = 0;
    genome.lineage = {};
    genome.fitness = 0.0;
    genome.age = 0;
    genome.actualization_level = 0.0;
    genome.birth_time = std::chrono::steady_clock::now();
    
    // Initialize ontological genes (structure)
    genome.genes.ontological = {
        {"sram_size", 0.5, 0.0, 1.0, true},
        {"flash_size", 0.5, 0.0, 1.0, true},
        {"register_density", 0.7, 0.0, 1.0, true}
    };
    
    // Initialize teleological genes (purpose)
    genome.genes.teleological = {
        {"optimization_target", 0.8, 0.0, 1.0, true},
        {"performance_priority", 0.7, 0.0, 1.0, true},
        {"quality_priority", 0.6, 0.0, 1.0, true}
    };
    
    // Initialize cognitive genes (reasoning)
    genome.genes.cognitive = {
        {"inference_strategy", 0.75, 0.0, 1.0, true},
        {"cache_policy", 0.6, 0.0, 1.0, true},
        {"batch_size_preference", 0.5, 0.0, 1.0, true}
    };
    
    // Initialize integrative genes (coherence)
    genome.genes.integrative = {
        {"driver_coupling", 0.85, 0.0, 1.0, true},
        {"interrupt_priority", 0.7, 0.0, 1.0, true},
        {"dma_strategy", 0.6, 0.0, 1.0, true}
    };
    
    // Initialize evolutionary genes (growth)
    genome.genes.evolutionary = {
        {"mutation_sensitivity", 0.15, 0.0, 0.5, true},
        {"learning_rate", 0.1, 0.0, 0.5, true},
        {"adaptation_speed", 0.2, 0.0, 1.0, true}
    };
    
    return genome;
}

// ============================================================================
// Genome Mutation
// ============================================================================

NPUGenome NPUOntogenesis::mutateGenome(const NPUGenome& parent, double mutation_rate) {
    NPUGenome mutated = parent;
    mutated.id = parent.id + "-mut-" + std::to_string(randomInt(1000, 9999));
    mutated.generation = parent.generation + 1;
    mutated.lineage = parent.lineage;
    mutated.lineage.push_back(parent.id);
    mutated.age = 0;
    mutated.birth_time = std::chrono::steady_clock::now();
    
    // Mutate genes with probability mutation_rate
    auto mutateGeneList = [mutation_rate](std::vector<NPUGene>& genes) {
        for (auto& gene : genes) {
            if (gene.mutable_ && randomDouble() < mutation_rate) {
                // Gaussian mutation
                double delta = randomDouble(-0.2, 0.2);
                gene.value = std::clamp(gene.value + delta, gene.min_value, gene.max_value);
            }
        }
    };
    
    mutateGeneList(mutated.genes.ontological);
    mutateGeneList(mutated.genes.teleological);
    mutateGeneList(mutated.genes.cognitive);
    mutateGeneList(mutated.genes.integrative);
    mutateGeneList(mutated.genes.evolutionary);
    
    return mutated;
}

// ============================================================================
// Genome Crossover
// ============================================================================

NPUGenome NPUOntogenesis::crossoverGenomes(const NPUGenome& parent1, const NPUGenome& parent2) {
    NPUGenome offspring;
    offspring.id = "hybrid-" + std::to_string(randomInt(10000, 99999));
    offspring.generation = std::max(parent1.generation, parent2.generation) + 1;
    offspring.lineage = {parent1.id, parent2.id};
    offspring.age = 0;
    offspring.birth_time = std::chrono::steady_clock::now();
    
    // Single-point crossover for each gene type
    auto crossoverGeneList = [](const std::vector<NPUGene>& genes1, 
                                const std::vector<NPUGene>& genes2) -> std::vector<NPUGene> {
        std::vector<NPUGene> result;
        size_t crossover_point = randomInt(0, std::min(genes1.size(), genes2.size()));
        
        for (size_t i = 0; i < std::max(genes1.size(), genes2.size()); ++i) {
            if (i < crossover_point && i < genes1.size()) {
                result.push_back(genes1[i]);
            } else if (i < genes2.size()) {
                result.push_back(genes2[i]);
            }
        }
        
        return result;
    };
    
    offspring.genes.ontological = crossoverGeneList(parent1.genes.ontological, parent2.genes.ontological);
    offspring.genes.teleological = crossoverGeneList(parent1.genes.teleological, parent2.genes.teleological);
    offspring.genes.cognitive = crossoverGeneList(parent1.genes.cognitive, parent2.genes.cognitive);
    offspring.genes.integrative = crossoverGeneList(parent1.genes.integrative, parent2.genes.integrative);
    offspring.genes.evolutionary = crossoverGeneList(parent1.genes.evolutionary, parent2.genes.evolutionary);
    
    return offspring;
}

// ============================================================================
// Fitness Assessment
// ============================================================================

FitnessMetrics NPUOntogenesis::assessFitness(const LlamaCoprocessorDriver* npu) {
    FitnessMetrics metrics;
    
    // Performance metrics (normalized 0.0-1.0)
    metrics.inference_speed = 0.75;      // Placeholder - would read actual tokens/sec
    metrics.throughput = 0.80;           // Placeholder - batch efficiency
    metrics.memory_efficiency = 0.85;    // Placeholder - RAM utilization
    metrics.gpu_efficiency = 0.70;       // Placeholder - GPU utilization
    metrics.stability = 0.90;            // Placeholder - error-free operation
    metrics.reliability = 0.95;          // Placeholder - uptime
    
    // Entelechy metrics
    auto assessment = NPUEntelechyIntrospector::assess(npu);
    metrics.actualization = assessment.overall_actualization;
    metrics.completeness = assessment.ontological.architectural_coherence;
    
    // Calculate overall fitness
    metrics.overall_fitness = calculateOverallFitness(metrics);
    
    return metrics;
}

double NPUOntogenesis::calculateOverallFitness(const FitnessMetrics& metrics) {
    return 
        metrics.inference_speed * 0.15 +
        metrics.throughput * 0.15 +
        metrics.memory_efficiency * 0.10 +
        metrics.gpu_efficiency * 0.10 +
        metrics.stability * 0.15 +
        metrics.reliability * 0.10 +
        metrics.actualization * 0.15 +
        metrics.completeness * 0.10;
}

// ============================================================================
// Self-Generation
// ============================================================================

std::shared_ptr<LlamaCoprocessorDriver> NPUOntogenesis::selfGenerate(
    const LlamaCoprocessorDriver& parent,
    double mutation_rate) {
    
    // Note: This is a conceptual implementation
    // In a real system, we would actually construct a new NPU instance
    // For now, we demonstrate the genome operations
    
    // Create offspring genome through mutation
    NPUGenome offspring_genome = mutateGenome(getGenome(parent), mutation_rate);
    
    // In production: would create actual LlamaCoprocessorDriver instance
    // and apply the genome to configure it
    auto offspring = std::make_shared<LlamaCoprocessorDriver>();
    offspring->genome_ = new NPUGenome(offspring_genome);
    
    // Apply genetic traits to configure the NPU
    applyGenomeToNPU(offspring.get(), offspring_genome);
    
    return offspring;
}

// ============================================================================
// Self-Optimization
// ============================================================================

void NPUOntogenesis::selfOptimize(LlamaCoprocessorDriver* npu, 
                                  int iterations,
                                  double learning_rate) {
    
    for (int i = 0; i < iterations; ++i) {
        // Assess current fitness
        auto current_metrics = assessFitness(npu);
        double current_fitness = current_metrics.overall_fitness;
        
        // Try optimization mutation
        NPUGenome optimized = getGenome(npu);
        
        // Gradient ascent on fitness landscape
        auto tryImproveGeneList = [learning_rate](std::vector<NPUGene>& genes) {
            for (auto& gene : genes) {
                if (gene.mutable_) {
                    // Small perturbation
                    double delta = randomDouble(-learning_rate, learning_rate);
                    gene.value = std::clamp(gene.value + delta, gene.min_value, gene.max_value);
                }
            }
        };
        
        tryImproveGeneList(optimized.genes.cognitive);
        tryImproveGeneList(optimized.genes.integrative);
        tryImproveGeneList(optimized.genes.evolutionary);
        
        // Apply and test
        NPUGenome backup = getGenome(npu);
        npu->genome_ = new NPUGenome(optimized);
        applyGenomeToNPU(npu, optimized);
        
        auto new_metrics = assessFitness(npu);
        double new_fitness = new_metrics.overall_fitness;
        
        // Keep if improvement, otherwise revert
        if (new_fitness > current_fitness) {
            recordDevelopmentEvent(npu, "Optimization improved fitness by " + 
                                  std::to_string((new_fitness - current_fitness) * 100.0) + "%");
        } else {
            delete npu->genome_;
            npu->genome_ = new NPUGenome(backup);
            applyGenomeToNPU(npu, backup);
        }
        
        // Update development stage
        updateDevelopmentStage(npu);
    }
}

// ============================================================================
// Self-Reproduction
// ============================================================================

std::shared_ptr<LlamaCoprocessorDriver> NPUOntogenesis::selfReproduce(
    const LlamaCoprocessorDriver& parent1,
    const LlamaCoprocessorDriver& parent2,
    const std::string& method) {
    
    auto offspring = std::make_shared<LlamaCoprocessorDriver>();
    
    if (method == "crossover") {
        NPUGenome result = crossoverGenomes(getGenome(parent1), getGenome(parent2));
        offspring->genome_ = new NPUGenome(result);
    } else if (method == "mutation") {
        NPUGenome result = mutateGenome(getGenome(parent1), 0.15);
        offspring->genome_ = new NPUGenome(result);
    } else {
        // Cloning
        NPUGenome clone = getGenome(parent1);
        clone.id = clone.id + "-clone";
        offspring->genome_ = new NPUGenome(clone);
    }
    
    applyGenomeToNPU(offspring.get(), *offspring->genome_);
    
    return offspring;
}

// ============================================================================
// Apply Genome to NPU
// ============================================================================

void NPUOntogenesis::applyGenomeToNPU(LlamaCoprocessorDriver* npu, const NPUGenome& genome) {
    // In a real implementation, this would configure the NPU based on genetic traits
    // For example:
    // - Ontological genes → memory sizes, register configurations
    // - Cognitive genes → inference strategies, cache policies
    // - Integrative genes → interrupt priorities, DMA settings
    
    // For now, just store the genome
    if (!npu->genome_) {
        npu->genome_ = new NPUGenome(genome);
    } else {
        *npu->genome_ = genome;
    }
}

// ============================================================================
// Development Management
// ============================================================================

void NPUOntogenesis::updateDevelopmentStage(LlamaCoprocessorDriver* npu) {
    auto& state = getState(npu);
    auto& genome = getGenome(npu);
    
    // Age the NPU
    genome.age++;
    
    // Update maturity based on fitness and age
    auto metrics = assessFitness(npu);
    state.maturity_level = std::min(100, static_cast<int>(metrics.overall_fitness * 100.0));
    state.vitality = metrics.stability;
    
    // Update stage based on maturity
    if (state.maturity_level < 30) {
        state.stage = OntogeneticStage::EMBRYONIC;
    } else if (state.maturity_level < 60) {
        state.stage = OntogeneticStage::JUVENILE;
    } else if (state.maturity_level < 90) {
        state.stage = OntogeneticStage::MATURE;
    } else {
        state.stage = OntogeneticStage::SENESCENT;
    }
}

bool NPUOntogenesis::canReproduce(const LlamaCoprocessorDriver& npu) {
    if (!npu.ontogenetic_state_) return false;
    return npu.ontogenetic_state_->stage == OntogeneticStage::MATURE &&
           npu.ontogenetic_state_->maturity_level >= 60;
}

void NPUOntogenesis::recordDevelopmentEvent(LlamaCoprocessorDriver* npu, const std::string& event) {
    auto& state = getState(npu);
    state.development_events.push_back(event);
    
    // Keep only last 100 events
    if (state.development_events.size() > 100) {
        state.development_events.erase(state.development_events.begin());
    }
}

// ============================================================================
// Population Evolution
// ============================================================================

std::vector<GenerationStats> NPUEvolution::evolvePopulation(
    const EvolutionConfig& config,
    const std::vector<std::shared_ptr<LlamaCoprocessorDriver>>& seed_population) {
    
    std::vector<GenerationStats> history;
    auto population = seed_population;
    
    // Expand seed population if needed
    while (population.size() < static_cast<size_t>(config.population_size)) {
        auto parent = population[randomInt(0, population.size() - 1)];
        auto offspring = NPUOntogenesis::selfGenerate(*parent, config.mutation_rate);
        population.push_back(offspring);
    }
    
    for (int gen = 0; gen < config.max_generations; ++gen) {
        // Evaluate fitness for all members
        std::vector<double> fitness_scores;
        for (const auto& npu : population) {
            auto metrics = NPUOntogenesis::assessFitness(npu.get());
            fitness_scores.push_back(metrics.overall_fitness);
        }
        
        // Record generation statistics
        GenerationStats stats;
        stats.generation = gen;
        stats.best_fitness = *std::max_element(fitness_scores.begin(), fitness_scores.end());
        stats.avg_fitness = std::accumulate(fitness_scores.begin(), fitness_scores.end(), 0.0) / fitness_scores.size();
        stats.worst_fitness = *std::min_element(fitness_scores.begin(), fitness_scores.end());
        stats.diversity = calculateDiversity(population);
        
        // Find best NPU
        auto best_idx = std::distance(fitness_scores.begin(), 
                                      std::max_element(fitness_scores.begin(), fitness_scores.end()));
        stats.best_npu_id = getGenome(*population[best_idx]).id;
        
        history.push_back(stats);
        
        // Check termination condition
        if (stats.best_fitness >= config.fitness_threshold) {
            break;
        }
        
        // Evolve to next generation
        population = evolveGeneration(population, fitness_scores, config);
    }
    
    return history;
}

std::vector<std::shared_ptr<LlamaCoprocessorDriver>> NPUEvolution::evolveGeneration(
    const std::vector<std::shared_ptr<LlamaCoprocessorDriver>>& population,
    const std::vector<double>& fitness_scores,
    const EvolutionConfig& config) {
    
    std::vector<std::shared_ptr<LlamaCoprocessorDriver>> next_generation;
    
    // Elitism: preserve top performers
    int elite_count = static_cast<int>(population.size() * config.elitism_rate);
    auto elite_indices = eliteSelection(fitness_scores, elite_count);
    for (int idx : elite_indices) {
        next_generation.push_back(population[idx]);
    }
    
    // Fill rest with offspring
    while (next_generation.size() < population.size()) {
        // Tournament selection for parents
        auto parent_indices = tournamentSelection(fitness_scores, 2, 3);
        
        std::shared_ptr<LlamaCoprocessorDriver> offspring;
        
        if (randomDouble() < config.crossover_rate) {
            // Crossover
            offspring = NPUOntogenesis::selfReproduce(
                *population[parent_indices[0]],
                *population[parent_indices[1]],
                "crossover");
        } else {
            // Mutation only
            offspring = NPUOntogenesis::selfGenerate(
                *population[parent_indices[0]],
                config.mutation_rate);
        }
        
        next_generation.push_back(offspring);
    }
    
    return next_generation;
}

// ============================================================================
// Selection Methods
// ============================================================================

std::vector<int> NPUEvolution::tournamentSelection(const std::vector<double>& fitness,
                                                   int count,
                                                   int tournament_size) {
    std::vector<int> selected;
    
    for (int i = 0; i < count; ++i) {
        // Pick random individuals for tournament
        std::vector<int> tournament;
        for (int j = 0; j < tournament_size; ++j) {
            tournament.push_back(randomInt(0, fitness.size() - 1));
        }
        
        // Select best from tournament
        int best = tournament[0];
        for (int idx : tournament) {
            if (fitness[idx] > fitness[best]) {
                best = idx;
            }
        }
        
        selected.push_back(best);
    }
    
    return selected;
}

std::vector<int> NPUEvolution::eliteSelection(const std::vector<double>& fitness, int count) {
    std::vector<int> indices(fitness.size());
    std::iota(indices.begin(), indices.end(), 0);
    
    std::partial_sort(indices.begin(), indices.begin() + count, indices.end(),
                     [&fitness](int a, int b) { return fitness[a] > fitness[b]; });
    
    return std::vector<int>(indices.begin(), indices.begin() + count);
}

// ============================================================================
// Diversity Calculation
// ============================================================================

double NPUEvolution::geneticDistance(const NPUGenome& genome1, const NPUGenome& genome2) {
    double distance = 0.0;
    int gene_count = 0;
    
    auto compareGeneLists = [&distance, &gene_count](const std::vector<NPUGene>& genes1,
                                                      const std::vector<NPUGene>& genes2) {
        for (size_t i = 0; i < std::min(genes1.size(), genes2.size()); ++i) {
            distance += std::abs(genes1[i].value - genes2[i].value);
            gene_count++;
        }
    };
    
    compareGeneLists(genome1.genes.ontological, genome2.genes.ontological);
    compareGeneLists(genome1.genes.teleological, genome2.genes.teleological);
    compareGeneLists(genome1.genes.cognitive, genome2.genes.cognitive);
    compareGeneLists(genome1.genes.integrative, genome2.genes.integrative);
    compareGeneLists(genome1.genes.evolutionary, genome2.genes.evolutionary);
    
    return gene_count > 0 ? distance / gene_count : 0.0;
}

double NPUEvolution::calculateDiversity(const std::vector<std::shared_ptr<LlamaCoprocessorDriver>>& population) {
    if (population.size() < 2) return 0.0;
    
    double total_distance = 0.0;
    int comparisons = 0;
    
    for (size_t i = 0; i < population.size(); ++i) {
        for (size_t j = i + 1; j < population.size(); ++j) {
            total_distance += geneticDistance(getGenome(*population[i]), getGenome(*population[j]));
            comparisons++;
        }
    }
    
    return total_distance / comparisons;
}

// ============================================================================
// Reporting
// ============================================================================

std::string NPUEvolution::formatGenerationReport(const GenerationStats& stats) {
    std::ostringstream oss;
    
    oss << "Generation " << stats.generation << ":\n";
    oss << "  Best:     " << std::fixed << std::setprecision(3) << stats.best_fitness << "\n";
    oss << "  Average:  " << stats.avg_fitness << "\n";
    oss << "  Worst:    " << stats.worst_fitness << "\n";
    oss << "  Diversity: " << stats.diversity << "\n";
    oss << "  Best NPU: " << stats.best_npu_id << "\n";
    
    return oss.str();
}

std::string NPUEvolution::formatEvolutionSummary(const std::vector<GenerationStats>& history) {
    std::ostringstream oss;
    
    oss << "\n╔═══════════════════════════════════════════════════════════╗\n";
    oss << "║        NPU EVOLUTION SUMMARY                            ║\n";
    oss << "╚═══════════════════════════════════════════════════════════╝\n\n";
    
    oss << "Total Generations: " << history.size() << "\n";
    oss << "Final Best Fitness: " << std::fixed << std::setprecision(3) 
        << history.back().best_fitness << "\n";
    oss << "Initial Best Fitness: " << history.front().best_fitness << "\n";
    oss << "Improvement: +" << ((history.back().best_fitness - history.front().best_fitness) * 100.0) 
        << "%\n\n";
    
    return oss.str();
}

// ============================================================================
// Self-Transcendence
// ============================================================================

bool NPUTranscendence::canTranscend(const LlamaCoprocessorDriver& npu) {
    auto assessment = NPUEntelechyIntrospector::assess(&npu);
    return assessment.overall_actualization >= 0.80;
}

void NPUTranscendence::enterTranscendence(LlamaCoprocessorDriver* npu) {
    if (!canTranscend(*npu)) {
        throw std::runtime_error("NPU not ready for transcendence (actualization < 80%)");
    }
    
    enableAutonomousGoals(npu);
    enableRecursiveOptimization(npu);
    enableEmergentDiscovery(npu);
}

void NPUTranscendence::enableAutonomousGoals(LlamaCoprocessorDriver* npu) {
    auto& trans = getTranscendence(npu);
    trans.autonomous_goals = true;
    NPUOntogenesis::recordDevelopmentEvent(npu, "Autonomous goal-setting enabled");
}

void NPUTranscendence::enableRecursiveOptimization(LlamaCoprocessorDriver* npu) {
    auto& trans = getTranscendence(npu);
    trans.recursive_optimization = true;
    NPUOntogenesis::recordDevelopmentEvent(npu, "Recursive self-optimization enabled");
}

void NPUTranscendence::enableEmergentDiscovery(LlamaCoprocessorDriver* npu) {
    auto& trans = getTranscendence(npu);
    trans.emergent_discovery = true;
    NPUOntogenesis::recordDevelopmentEvent(npu, "Emergent capability discovery enabled");
}

TranscendenceCapabilities NPUTranscendence::assessTranscendence(const LlamaCoprocessorDriver* npu) {
    if (!npu->transcendence_) {
        TranscendenceCapabilities empty{};
        return empty;
    }
    return *npu->transcendence_;
}

} // namespace vdev
} // namespace ggnucash
