# ECE 563 Project 1: Cache Simulator

## Overview
This project implements a comprehensive cache simulator that models both L1 and L2 cache hierarchies with various replacement policies, prefetching strategies, and associativity configurations. The simulator analyzes memory access patterns and provides detailed performance metrics.

## Features
- **Multi-level Cache Hierarchy**: Configurable L1 and L2 caches
- **Flexible Associativity**: Support for different associativity levels (1, 2, 4, 8, fully associative)
- **Replacement Policies**: LRU (Least Recently Used) replacement
- **Prefetching**: Stream buffer-based prefetching with configurable parameters
- **Performance Analysis**: Detailed hit/miss statistics and memory access patterns

## Project Structure
```
Project_1/
├── read-trace/
│   ├── c_files/          # C implementation
│   │   ├── Makefile
│   │   ├── sim.c/h       # Cache simulator source
│   │   └── traces/       # Sample trace files
│   └── cpp_files/        # C++ implementation (main)
│       ├── Makefile
│       ├── sim.cc/h      # Cache simulator source
│       ├── cache*.cc/h   # Cache hierarchy implementation
│       ├── stream*.cc/h  # Prefetching implementation
│       ├── simulation_data/    # Performance data for different configs
│       ├── traces/       # Trace files (gcc, perl, compress, etc.)
│       └── *.py          # Analysis and plotting scripts
├── traces/               # Additional trace files
├── val*.txt              # Validation output files
└── *.zip                 # Project archives
```

## Build Instructions

### C++ Implementation (Recommended)
```bash
cd read-trace/cpp_files
make clean
make
```

### C Implementation
```bash
cd read-trace/c_files
make clean
make
```

## Usage

### Command Line Format
```bash
./sim <BLOCKSIZE> <L1_SIZE> <L1_ASSOC> <L2_SIZE> <L2_ASSOC> <PREF_N> <PREF_M> <trace_file>
```

### Parameters
- `BLOCKSIZE`: Cache block size in bytes
- `L1_SIZE`: L1 cache size in bytes
- `L1_ASSOC`: L1 associativity (1, 2, 4, 8, or 0 for fully associative)
- `L2_SIZE`: L2 cache size in bytes
- `L2_ASSOC`: L2 associativity (1, 2, 4, 8, or 0 for fully associative)
- `PREF_N`: Stream prefetcher degree (number of subsequent blocks to prefetch)
- `PREF_M`: Stream prefetcher distance (blocks ahead to prefetch)
- `trace_file`: Path to memory trace file

### Example Usage
```bash
# Basic configuration: 32B blocks, 8KB L1 (4-way), 256KB L2 (8-way), prefetching enabled
./sim 32 8192 4 262144 8 3 10 gcc_trace.txt

# No prefetching
./sim 32 8192 4 262144 8 0 0 gcc_trace.txt

# View output with less (recommended for long traces)
./sim 32 8192 4 262144 8 3 10 gcc_trace.txt | less
```

## Trace Files
The project includes several benchmark traces:
- `gcc_trace.txt`: GCC compilation trace
- `perl_trace.txt`: Perl execution trace
- `compress_trace.txt`: Compression utility trace
- `go_trace.txt`: Go compiler trace
- `vortex_trace.txt`: Vortex benchmark trace

## Key Results and Analysis

### Cache Size vs Performance
The simulator generates comprehensive data showing how cache size affects hit rates across different benchmarks. Key findings include:
- Performance improvements diminish with cache size increases
- Different applications show varying sensitivity to cache parameters

### Prefetching Impact
Stream buffer prefetching provides significant performance improvements for applications with good spatial locality, though it can hurt performance for applications with poor prefetch accuracy.

### Validation
The project includes validation files (`val*.txt`) that verify simulator correctness against expected outputs for various configurations.

## Dependencies
- C++ compiler (g++ recommended)
- Make build system
- Python 3 (for analysis scripts)

## Output Format
The simulator outputs detailed statistics including:
- Cache configuration summary
- L1 and L2 hit/miss counts and rates
- Memory traffic analysis
- Prefetcher performance metrics

## Academic Context
This project is part of ECE 563: Computer Architecture, focusing on memory hierarchy design and performance analysis. The simulator demonstrates fundamental concepts in cache design, replacement policies, and prefetching strategies.
