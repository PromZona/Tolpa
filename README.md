# TOLPA

Tolpa is a game-like demo simulation of fantasy world map. This project is heavily influenced by global map of Mount and Blade game series.
Main purpose of the project for developers is to master C++ skills and try different architectural patterns commonly used in game development. 

## How to build
Tolpa uses CMake as a build manager. Minimum required version of CMake is 3.11
```bash
mkdir build
cd build
cmake ..
cmake --build .
```
Find and run executable in the build directory.

## Simulation Rules
Simulation is defined by set of rules which describe how each entity should behave itself in different situations and how they can interact with each other

[Documentation](docs/SimulationRules.md)

## ECS
Tolpa has custom ECS implementation. It was created from scratch for study purposes. 
Archetype version of ECS was used. Additional information can be found on the documentation page.

[Documentation](src/ECS/README.md)