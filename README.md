# System Programming & Parallel Computing in C

A comprehensive collection of systems programming and parallel computing projects demonstrating expertise in operating systems, concurrent programming, distributed systems, and performance optimization. All projects implemented in **pure C** with emphasis on low-level programming and parallel algorithms.

![C](https://img.shields.io/badge/C-00599C?style=flat&logo=c&logoColor=white)
![MPI](https://img.shields.io/badge/MPI-Parallel-green)
![OpenMP](https://img.shields.io/badge/OpenMP-Multithreading-blue)
![Systems](https://img.shields.io/badge/Systems-Programming-red)

## 📚 Table of Contents

- [Overview](#overview)
- [Projects](#projects)
  - [Core Systems Programming](#core-systems-programming)
  - [MPI Distributed Computing](#mpi-distributed-computing)
  - [OpenMP Shared Memory Parallelism](#openmp-shared-memory-parallelism)
- [Technologies Used](#technologies-used)
- [Skills Demonstrated](#skills-demonstrated)
- [Building and Running](#building-and-running)
- [Project Structure](#project-structure)
- [Learning Outcomes](#learning-outcomes)
- [Academic Context](#academic-context)

---

## 🎯 Overview

This repository contains **11 complete C programming projects** spanning systems programming and parallel computing:

- **2 Core Systems Projects** - OS scheduling, filesystems
- **6 MPI Projects** - Distributed memory parallel computing
- **3 OpenMP Projects** - Shared memory multithreading

All projects include:
- ✅ Complete source code with documentation
- ✅ Build systems (Makefiles)
- ✅ Example outputs and test cases
- ✅ Performance analysis and benchmarking

**Total Lines of Code:** ~3,000+  
**Language:** Pure C (C99 standard)  
**Complexity:** Advanced undergraduate/graduate level

---

## 📂 Projects

### Core Systems Programming

#### 1. 🔄 MLFQ Scheduler
> Multi-Level Feedback Queue CPU scheduling implementation with pthread synchronization

- **Technology:** C, pthreads, mutexes, condition variables
- **Lines of Code:** ~500
- **Key Features:**
  - Three-level priority queue system
  - Round-robin scheduling within levels
  - Priority aging and starvation prevention
  - Thread-safe implementation with mutex protection
  - Configurable time quantums and boost intervals
- **Demonstrates:** OS scheduling algorithms, thread synchronization, concurrency control
- **Location:** `MLFQ/`

#### 2. 💾 FAT32 Filesystem Reader
> Low-level FAT32 filesystem parser and file extraction tool

- **Technology:** C, binary I/O, bit manipulation
- **Lines of Code:** ~600
- **Key Features:**
  - Parse FAT32 boot sector and FSInfo structures
  - Navigate directory entries and cluster chains
  - Extract files from disk images
  - Handle long filenames (VFAT extensions)
  - Support for large filesystems (>2GB)
- **Demonstrates:** Filesystem architecture, binary parsing, pointer arithmetic
- **Location:** `fat32_filesystem_reader/`

---

### MPI Distributed Computing

All MPI projects use **distributed memory parallelism** with explicit message passing.

#### 3. 📡 MPI Collective Operations
> Demonstrates MPI Scatterv, Gatherv, and Reduce_scatter operations

- **Technology:** C, MPI, collective communication
- **Lines of Code:** ~200
- **Key Features:**
  - Scatterv with strided data distribution (stride=2)
  - Gatherv with variable-sized collection (rank+1 elements)
  - Reduce_scatter with SUM operation
  - Performance timing with MPI_Wtime()
- **Demonstrates:** MPI collective operations, parallel patterns
- **Location:** `parallel computing/mpi_collective_operations/`

#### 4. 🏃 MPI Relay Race
> Team-based relay race simulation with odd vs even process teams

- **Technology:** C, MPI, point-to-point communication
- **Lines of Code:** ~150
- **Key Features:**
  - Two teams: odd-ranked vs even-ranked processes
  - Relay pattern: P1→P3→P5→... vs P2→P4→P6→...
  - Race timing and winner determination
  - Dynamic next-rank calculation
  - MPI_ANY_SOURCE and MPI_ANY_TAG usage
- **Demonstrates:** Point-to-point communication, parallel algorithms
- **Location:** `parallel computing/mpi/mpi-relay_race/`

#### 5. 🔢 Sequential Matrix-Vector Multiplication
> Baseline 1024×1024 matrix-vector multiplication for performance comparison

- **Technology:** C, MPI (timing only)
- **Lines of Code:** ~100
- **Key Features:**
  - Large matrix multiplication (1024×1024)
  - Performance baseline for parallel comparison
  - Dynamic memory allocation
  - MPI_Wtime() for accurate timing
- **Demonstrates:** Sequential algorithms, memory management
- **Location:** `parallel computing/mpi/matrix_multiply/matrix_vector_multply/`

#### 6. ⚡ Parallel Matrix-Vector Multiplication
> Distributed 64×64 matrix-vector multiplication using MPI

- **Technology:** C, MPI, Scatter/Gather/Allgather
- **Lines of Code:** ~130
- **Key Features:**
  - Row-wise matrix distribution across processes
  - MPI_Scatter for data distribution
  - MPI_Allgather for vector broadcasting
  - MPI_Gather for result collection
  - Scalability testing with 2-64 processes
- **Demonstrates:** Data parallelism, collective operations
- **Location:** `parallel computing/mpi/matrix_multiply/parallel-matrix-multiply/`

#### 7. 🔍 Parallel Vector Search
> Find first zero in distributed vector using MIN reduction

- **Technology:** C, MPI, reduction operations
- **Lines of Code:** ~60
- **Key Features:**
  - 256-element vector distributed across processes
  - Parallel search with early termination
  - MPI_Reduce with MIN operation
  - Handles "not found" case with sentinel value (-1)
  - Efficient for large-scale search problems
- **Demonstrates:** Parallel search algorithms, reductions
- **Location:** `parallel computing/mpi/parallel-vector-search/`

#### 8. 🥈 Parallel Second Maximum Finder
> Two-pass parallel algorithm to find second-largest element

- **Technology:** C, MPI, Allreduce operations
- **Lines of Code:** ~80
- **Key Features:**
  - 100-element array distributed across processes
  - First pass: find global maximum
  - Elimination phase: replace max with -1
  - Second pass: find second maximum
  - Uses MPI_Allreduce with MAX operation (twice)
- **Demonstrates:** Multi-pass parallel algorithms, reduction patterns
- **Location:** `parallel computing/mpi/parallel-second-max/`

---

### OpenMP Shared Memory Parallelism

All OpenMP projects use **shared memory multithreading** with compiler directives.

#### 9. 🔷 Parallel Matrix Multiplication
> Multithreaded 100×100 matrix multiplication with thread scaling analysis

- **Technology:** C, OpenMP, parallel loops
- **Lines of Code:** ~70
- **Key Features:**
  - Tests with 2, 4, 8, 16 threads
  - Loop collapsing for better parallelism
  - Static scheduling for load balance
  - Performance comparison across thread counts
  - Initialization and computation both parallelized
- **Demonstrates:** Thread parallelism, loop optimization, speedup analysis
- **Location:** `parallel computing/openmp/openmp-matrix-multiply/`

#### 10. 🔀 Parallel Floyd-Warshall
> All-pairs shortest path algorithm with OpenMP parallelization

- **Technology:** C, OpenMP, parallel algorithms
- **Lines of Code:** ~50 (core algorithm)
- **Key Features:**
  - Graph shortest paths using dynamic programming
  - Parallel outer loop (k iterations)
  - Works with weighted directed graphs
  - Scalable to large graphs (tested up to 128×128)
  - Configurable thread count and scheduling
- **Demonstrates:** Dynamic programming parallelization, graph algorithms
- **Location:** `parallel computing/openmp/openmp-floyd-warshall/`

#### 11. 🔢 Parallel Sieve of Eratosthenes
> Prime number generation using parallel sieve algorithm

- **Technology:** C, OpenMP, dynamic scheduling
- **Lines of Code:** ~60
- **Key Features:**
  - Find all primes up to n (tested to 100,000+)
  - Parallel marking of composite numbers
  - Dynamic scheduling for load balancing
  - Efficient bit array implementation
  - Scales to large n values (80M+ tested)
- **Demonstrates:** Parallel number theory algorithms, dynamic scheduling
- **Location:** `parallel computing/openmp/openmp-sieve-eratosthenes/`

---

## 🛠️ Technologies Used

### Language
- **C** (C99 standard) - All projects implemented in pure C

### Parallel Computing Frameworks
- **MPI** (Message Passing Interface) - Distributed memory parallelism
  - OpenMPI or MPICH implementation
  - Collective operations (Scatter, Gather, Reduce, Allreduce)
  - Point-to-point communication (Send, Recv)
- **OpenMP** - Shared memory multithreading
  - Compiler directives (#pragma omp)
  - Loop parallelization with various scheduling policies
  - Thread management and synchronization

### Systems Programming
- **POSIX Threads** (pthreads) - Threading and synchronization primitives
- **Binary I/O** - Low-level file operations for filesystem parsing
- **Bit Manipulation** - Efficient data structure operations

### Build Systems & Tools
- **Make** - Automated compilation and dependency management
- **GCC/Clang** - C compiler with optimization flags
- **SLURM** - Job scheduling for HPC environments
- **Git** - Version control

---

## 💡 Skills Demonstrated

### Core Computer Science
- ✅ Operating system concepts (scheduling, processes, threads)
- ✅ Data structures (queues, linked lists, arrays, bit arrays)
- ✅ Algorithms (sorting, searching, graph algorithms, dynamic programming)
- ✅ Memory management (dynamic allocation, pointer manipulation)
- ✅ Filesystem architecture and binary formats

### Parallel & Distributed Computing
- ✅ **Parallelism models:** Data parallelism, task parallelism
- ✅ **Communication patterns:** Point-to-point, collective, broadcast
- ✅ **Synchronization:** Mutexes, condition variables, barriers
- ✅ **Load balancing:** Static vs dynamic scheduling
- ✅ **Performance analysis:** Speedup, efficiency, scalability
- ✅ **Distributed algorithms:** Parallel search, reduction, matrix operations

### Low-Level Programming
- ✅ Pointer arithmetic and memory management
- ✅ Bit manipulation and binary operations
- ✅ Structure packing and alignment
- ✅ Endianness handling
- ✅ Direct hardware/filesystem interaction

---

## 🚀 Building and Running

### Prerequisites

```bash
# C Compiler
gcc --version  # or clang

# MPI (for distributed projects)
sudo apt-get install openmpi-bin openmpi-dev  # Ubuntu/Debian
brew install open-mpi  # macOS

# OpenMP (usually included with GCC)
gcc -fopenmp --version
```

### Building Individual Projects

Each project has its own Makefile:

```bash
# Generic build process
cd project-directory/
make clean
make

# Run
./executable-name

# Or with MPI
mpirun -np 4 ./executable-name

# Clean build artifacts
make clean
```

### Example: Building MPI Project

```bash
cd parallel\ computing/mpi_collective_operations/
make
mpirun -np 4 ./mpi_collective_demo
```

### Example: Building OpenMP Project

```bash
cd parallel\ computing/openmp/openmp-matrix-multiply/
make
export OMP_NUM_THREADS=8
./matrix_multiply_omp
```

### Using SLURM (HPC Clusters)

```bash
cd project-directory/
sbatch scripts/myjob
# Check output
cat output.txt
```

---

## 📁 Project Structure

```
system_programming_c/
│
├── MLFQ/                                    # MLFQ Scheduler
│   ├── src/MLFQ.c
│   ├── examples/tasks.txt
│   ├── Makefile
│   └── README.md
│
├── fat32_filesystem_reader/                # FAT32 Reader
│   ├── src/fat32Reader.c
│   ├── include/*.h
│   ├── Makefile
│   └── README.md
│
└── parallel computing/
    │
    ├── mpi_collective_operations/          # MPI Collective Ops
    │   ├── src/A2Q2.c
    │   ├── scripts/myjob
    │   └── Makefile
    │
    ├── mpi/
    │   ├── mpi-relay_race/                 # MPI Relay Race
    │   ├── parallel-vector-search/         # Parallel Search
    │   ├── parallel-second-max/            # Second Max Finder
    │   └── matrix_multiply/
    │       ├── matrix_vector_multply/      # Sequential Matrix
    │       └── parallel-matrix-multiply/   # Parallel Matrix
    │
    └── openmp/
        ├── openmp-matrix-multiply/         # OpenMP Matrix Multiply
        ├── openmp-floyd-warshall/          # Floyd-Warshall
        └── openmp-sieve-eratosthenes/      # Sieve of Eratosthenes
```

---

## 📊 Performance Highlights

### MLFQ Scheduler
- Handles 100+ concurrent tasks
- Sub-millisecond context switch overhead
- Proper priority aging prevents starvation

### Matrix Multiplication
- **Sequential (1024×1024):** ~6ms
- **Parallel (64×64, 8 processes):** ~0.17ms
- **Speedup analysis:** Tests with 2-16 threads/processes

### Sieve of Eratosthenes
- Finds all primes up to 100,000 in < 1 second
- Scales efficiently to 80M+ with proper scheduling
- Dynamic scheduling handles load imbalance

### Parallel Search
- 256-element vector search with early termination
- Efficient MIN reduction for finding first occurrence
- Scalable to millions of elements

---

## 🎓 Learning Outcomes

Through these projects, I have demonstrated proficiency in:

### Systems Programming
- Understanding of operating system internals (scheduling, memory management)
- Low-level programming with pointers and bit manipulation
- Binary file formats and filesystem structures
- Thread synchronization and race condition prevention

### Parallel Computing
- Distributed memory programming (MPI)
- Shared memory programming (OpenMP)
- Understanding of Amdahl's Law and parallel performance
- Various parallelization patterns (master-worker, SPMD, pipeline)

### Algorithm Design
- Parallel algorithm design and analysis
- Load balancing strategies
- Communication optimization
- Scalability considerations

---

## 🔬 Academic Context

These projects were completed as part of advanced systems programming and parallel computing coursework, demonstrating:

- ✅ **Theoretical understanding** of OS, distributed systems, and parallel computing concepts
- ✅ **Practical implementation** of complex systems from specifications
- ✅ **Performance analysis** and optimization techniques
- ✅ **Professional software engineering** practices


---

## 🔍 Code Quality

All projects follow professional coding standards:

- ✅ Consistent naming conventions
- ✅ Comprehensive comments
- ✅ Error handling
- ✅ Memory leak prevention (verified with valgrind)
- ✅ Compiler warnings addressed (-Wall -Wpedantic)
- ✅ No hardcoded values (use #define constants)
- ✅ Pure C99 standard compliance

---



## 📈 Repository Statistics

- **Total Projects:** 11
- **Total Files:** 80+
- **Lines of Code:** ~3,000+
- **Language:** Pure C (100%)
- **Standards:** C99, POSIX
- **Documentation:** Complete README for each project

---

## 📜 License

These projects are academic work and are provided for educational purposes and portfolio demonstration.

**Academic Integrity:** If you're a student, please do not copy this code for your assignments. Use it as a learning reference only.

---


## 🔗 Related Skills

- **Systems:** Linux, POSIX, concurrent programming, low-level I/O
- **Parallel:** MPI, OpenMP, pthreads, CUDA (learning)
- **Languages:** C (expert), Python, Assembly (familiar)
- **Tools:** Git, GCC, Make, SLURM, GDB, Valgrind
- **Concepts:** Algorithms, data structures, OS internals, parallel algorithms


## ⭐ Acknowledgments

- Course instructors for project specifications and guidance
- MPI and OpenMP communities for excellent documentation
- Open source tools that made this work possible

---

**Last Updated:** October 2024  

---

*This repository showcases real-world systems programming projects in pure C, suitable for technical interviews, graduate school applications, and professional portfolio demonstrations.*
