# 42 so_long Project

Welcome to the 42 so_long project! This game is created as part of the 42 curriculum and showcases a range of exciting features and technical implementations.

![so_long Screenshot](https://github.com/MykleR/so_long/blob/main/resources/Screenshot%20from%202025-04-04%2018-13-12.png)

## ✨ Features

### Scene-Based Implementation
Scene-based implementation. The game allows you to load and unload scenes seamlessly. Each scene is defined by the memory size needed and four function pointers:
- `on_init`: Initializes the scene.
- `on_event`: Handles events within the scene.
- `on_update`: Updates the state of the scene.
- `on_clear`: Cleans up resources when the scene is unloaded.

### Custom ECS (Entity Component System)
For the bonus part of the project, I have integrated a custom ECS implementation written by myself in C. This system is the backbone of the game's architecture, providing flexible and efficient management of game entities and their components.
The Entity Component System (ECS) architecture is a powerful pattern commonly used in game development. It decouples data and behavior, allowing for greater flexibility and scalability. By using ECS, I can easily manage complex game logic, optimize performance, and facilitate code reuse. The ECS system processes entities through pre-built queries, ensuring O(1) quick access and efficient gameplay mechanics.

### Optimized Collision Detection
Our game features fully generic AABB (Axis-Aligned Bounding Box) optimized collision detection with custom replaceable response handlers. This allows for efficient collision checks and responses between various entities such as:
- Player and walls
- Player and bullets
- Bullets and walls
- Bullets and bullets

### ECS Components and Queries
All gameplay elements are managed using flexible ECS components and processed through pre-built queries for O(1) quick access. This ensures smooth and efficient game mechanics.

### Animations
like all other parts of the game, animations are ECS components, so they can be added to a new entity type very quickly. Only the player and the walls have no animations (Graphics are a mix of several free asset packs).

### Responsive Camera
The game includes a responsive camera that follows the player, providing an immersive experience as you navigate through the level.

## 🚀 Getting Started

### Prerequisites

- Clang compiler
- GNU Make
- SDL2 and Vulkan for [MacroLibx](https://github.com/seekrs/MacroLibX)

### Installation

```sh
# Clone the repository
git clone --recurse-submodules https://github.com/MykleR/so_long.git

# Enter the directory and compile project
cd so_long; make re bonus

# Have fun
./so_long_bonus resources/maps/platformer.ber
```
