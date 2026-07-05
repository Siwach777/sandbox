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

## Tech Stack
- **Language**: C++20
- **Build System**: CMake & Make
- **Graphics & Windowing**: SFML 3
- **UI Toolkit**: ImGui (with ImGui-SFML binding)
- **Architecture**: Custom Entity Component System (ECS)

## Project Structure
- `include/`: Contains all header files (`.hpp`), reflecting the source directory structure.
- `src/`: Contains all source files (`.cpp`).
  - `ECS/`: Core Entity Component System architecture (Entities, Components, World).
  - `Systems/`: Logic systems that operate on entities (e.g., Movement, Render, PheromoneSense).
  - `UI/`: User interface and ImGui debug panels.
  - `World/`: Environment systems such as the pheromone grid.
- `assets/`: Static resources for the simulation.
