# Image Segmentation using Max-Flow / Min-Cut

![Language](https://img.shields.io/badge/language-C++-00599C?style=flat-square&logo=c%2B%2B)
![Platform](https://img.shields.io/badge/platform-Linux-FCC624?style=flat-square&logo=linux)
![Build](https://img.shields.io/badge/build-Makefile-green?style=flat-square)

> Graph-based binary image segmentation algorithm using max-flow/min-cut principles, implemented in C++ for grayscale PGM images.

---

## Table of Contents

- [Overview](#overview)
- [Algorithm](#algorithm)
- [Project Structure](#project-structure)
- [Technologies](#technologies)
- [Build & Run](#build--run)
- [Input / Output Format](#input--output-format)
- [Skills Demonstrated](#skills-demonstrated)

---

## Overview

This project implements an **image segmentation** algorithm based on **graph theory** and **network flow**. The program takes a grayscale image in PGM format as input and produces a binary black-and-white segmented image by finding the optimal cut in a graph built from pixel intensities.

The segmentation problem is modelled as a **minimum s-t cut** in a directed weighted graph:
- Each pixel becomes a node
- Edges connect neighbouring pixels (4-connectivity)
- A **source node** (s) represents the foreground
- A **sink node** (t) represents the background
- Edge capacities encode pixel similarity

The algorithm finds the **maximum flow** from source to sink, whose value equals the **minimum cut** by the max-flow min-cut theorem, partitioning pixels into foreground/background.

---

## Algorithm

### Graph Construction

```
For each pixel p at position (i, j):
  - Add node v_p to graph
  - Add edge (s -> v_p) with capacity based on background likelihood
  - Add edge (v_p -> t) with capacity based on foreground likelihood
  - For each neighbour q of p:
      Add edge (v_p -> v_q) with capacity = similarity(p, q)
```

### Max-Flow (Ford-Fulkerson / BFS)

1. Initialise flow f = 0
2. While an augmenting path exists from s to t (BFS / DFS):
   - Find bottleneck capacity along the path
   - Augment flow along the path and update residual graph
3. Return total flow = value of minimum cut

### Segmentation

After max-flow terminates, perform a BFS/DFS from source on the **residual graph**:
- Nodes reachable from s → **foreground** (white)
- Nodes not reachable from s → **background** (black)

**Time complexity:** O(V · E²) in the worst case (Edmonds-Karp variant: O(V · E²))

---

## Project Structure

```
image-segmentation-maxflow/
├── src/
│   ├── main.cpp              # Entry point: argument parsing, I/O
│   ├── graph.hpp             # Graph abstract data structure (header)
│   ├── graph.cpp             # Graph implementation: add_edge, BFS
│   ├── maxflow.hpp           # Max-flow algorithm interface
│   ├── maxflow.cpp           # Ford-Fulkerson / Edmonds-Karp implementation
│   ├── image.hpp             # PGM image reader/writer interface
│   └── image.cpp             # PGM parser and binary output writer
├── images/
│   ├── input/                # Sample PGM grayscale input images
│   └── output/               # Segmented binary output images
├── tests/
│   └── test_graph.cpp        # Unit tests for graph and flow functions
├── Makefile                  # Build system
└── README.md
```

---

## Technologies

| Technology | Role |
|------------|------|
| C++17 | Core implementation language |
| Linux | Development and execution environment |
| Makefile | Build automation |
| PGM format | Grayscale image input/output |
| Git | Version control |

---

## Build & Run

### Prerequisites

- GCC ≥ 9 or Clang ≥ 10
- Linux (or WSL on Windows)
- Make

### Compile

```bash
# Clone the repository
git clone https://github.com/anastasia638/image-segmentation-maxflow.git
cd image-segmentation-maxflow

# Build using Makefile
make
```

### Run

```bash
# Segment an image
./segment images/input/sample.pgm images/output/result.pgm

# Clean build artifacts
make clean
```

### Makefile targets

```makefile
make          # Compile all sources into ./segment binary
make clean    # Remove compiled objects and binary
make test     # Compile and run unit tests
```

---

## Input / Output Format

**Input:** PGM (Portable Graymap) — ASCII or binary, grayscale 8-bit image.

```
P2
# Example PGM header
4 4
255
0 50 200 255
...
```

**Output:** PGM binary image where:
- `255` (white) = foreground pixel
- `0` (black) = background pixel

---

## Skills Demonstrated

- **Graph algorithms:** construction, BFS/DFS traversal, residual graph management
- **Max-flow / Min-cut:** Ford-Fulkerson algorithm, Edmonds-Karp optimisation
- **C++ programming:** pointers, dynamic memory, STL containers, modular design
- **Abstract data structures:** adjacency lists, edge representation
- **Image processing:** PGM format parsing, pixel-level manipulation
- **Linux development:** command-line tools, GCC compilation pipeline
- **Build systems:** Makefile with dependency tracking
- **Modular programming:** separation of concerns across header/source files

---

## Author

**Meriem Silmi** — Computer Science Student, France

[![GitHub](https://img.shields.io/badge/GitHub-anastasia638-black?style=flat-square&logo=github)](https://github.com/anastasia638)
