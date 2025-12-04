# Project CLAUDE.md

## Overview

This repository contains AI agent development work and cognitive architecture implementations.

## Project Structure

- `agents/` - Agent specifications, cognitive architecture docs, and persona definitions (see `agents/CLAUDE.md` for details)
- `opencog-modern/` - Modern C++23 reimplementation of OpenCog core
- `.claude/` - Claude Code configuration

## opencog-modern

A modernized C++23 implementation of OpenCog's cognitive architecture featuring:

- **Data-Oriented Design**: SoA layout for cache efficiency
- **Lock-Free Concurrency**: Atomic operations throughout
- **C++20/23**: Concepts, coroutines, ranges
- **SIMD PLN**: Vectorized truth value computation

### Building opencog-modern

```bash
cd opencog-modern
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build
```

### Key Components

| Component | Location | Purpose |
|-----------|----------|---------|
| Core Types | `include/opencog/core/` | AtomId, TruthValue, AttentionValue (8 bytes each) |
| AtomSpace | `include/opencog/atomspace/` | SoA storage, O(1) lookup |
| ECAN | `include/opencog/attention/` | Lock-free attention bank |
| Pattern Matcher | `include/opencog/pattern/` | Coroutine-based lazy matching |
| PLN | `include/opencog/pln/` | SIMD formulas, inference engine |
| URE | `include/opencog/ure/` | Unified Rule Engine |

## Development Conventions

- Use Markdown for documentation
- C++23 for implementation code (opencog-modern)
- Keep specification documents in `agents/`
- Follow existing file naming patterns

## Key Context

This project involves AI agent architectures including:
- Deep Tree Echo persona/cognitive system
- CogPrime/OpenCog foundations (modernized in `opencog-modern/`)
- Cosmos System 5 hierarchical state machines
- Supporting systems (Marduk, Neuro, SkinTwin)

## Notes

- The `opencog-modern` implementation prioritizes performance through cache-friendly memory layouts
- Pattern matching uses C++20 coroutines for lazy evaluation
- PLN formulas support AVX2 SIMD for batch operations
