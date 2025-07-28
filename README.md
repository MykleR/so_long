<div align="center">

<img src="https://github.com/MykleR/so_long/blob/main/so_long.png" width="300" />

##

<img src="https://github.com/MykleR/so_long/blob/main/resources/Screenshot%20from%202025-04-04%2018-13-12.png" width="800" />

</div>

## ✨ Features

### Scene-Based Implementation
> The game allows you to load and unload scenes seamlessly. A scene is defined by the memory needed and four function pointers:
> - `on_init`: Initializes the scene.
> - `on_event`: Handles events within the scene.
> - `on_update`: Updates the state of the scene.
> - `on_clear`: Cleans up resources when the scene is unloaded.

### Custom ECS (Entity Component System)

> The ECS architecture is a powerful pattern commonly used in game development. This system is the backbone of the game's architecture, providing flexible and efficient management of game entities and their components. By using an ECS, I can easily manage complex game logic, optimize performance, and facilitate code reuse. You can find bellow my custom implementation written in C.

[![Readme Card](https://github-readme-stats.vercel.app/api/pin/?username=MykleR&repo=42-ecs&theme=dark&bg_color=30,090a0c,303b51\&title_color=fff\&text_color=fff)](https://github.com/MykleR/42-ecs)

### Optimized Collision Detection
> The game features fully generic AABB (Axis-Aligned Bounding Box) optimized collision detection with custom replaceable response handlers. This allows for efficient collision checks and responses between various entities such as: Player and walls | Player and bullets | Bullets and walls | Bullets and bullets

### Animations
> like all other parts of the game, animations are ECS components, so they can be added to a new entity type very quickly. Only the player and the walls have no animations (Graphics are a mix of several free asset packs).

### Responsive Camera
> The game includes a responsive smooth camera that follows the player, provides an example of a basic 2D camera implementation.

## 🚀 Getting Started

### Prerequisites

> - Clang compiler
> - GNU Make
> - SDL2 and Vulkan for [MacroLibx](https://github.com/seekrs/MacroLibX)

### Installation

```sh
# Clone the repository
git clone --recurse-submodules https://github.com/MykleR/so_long.git

# Enter the directory and compile project
cd so_long; make re bonus

# Have fun
./so_long_bonus resources/maps/platformer.ber
```
> [!IMPORTANT]\
> Don't forget `--recurse-submodules` otherwise dependencies will not be cloned
