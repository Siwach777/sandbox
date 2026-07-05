# World Sim (Sandbox)

## Aim of the Project
The primary goal of this project is to build a highly optimized, ECS-based (Entity Component System) simulation world. It is designed to model environments where agents, such as ants, coexist and interact with one another and their surroundings. By simulating these localized interactions at scale, the project serves as a framework to study and observe emergent behavior and complex systems in real-time.

## Prerequisites
To build and run the simulation, you will need:
- A C++20 compatible compiler
- CMake (version 3.20 or higher)
- Make
- SFML 3 and ImGui (these are fetched automatically via CMake if not installed locally)

## How to Build and Run

1. **Configure the Project**  
   Create the build directory and generate the build files using CMake:
   ```bash
   cmake -B build
   ```

2. **Compile the Code**  
   Use the provided Makefile to compile the project in parallel:
   ```bash
   make
   ```

3. **Run the Simulation**  
   Execute the compiled binary from the root directory:
   ```bash
   ./build/world_sim
   ```
