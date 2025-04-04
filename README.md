# 42 so_long Project

Welcome to the 42 so_long project! This game is created as part of the 42 curriculum and showcases a range of exciting features and technical implementations.

![so_long Screenshot](https://user-images.githubusercontent.com/123456789/so_long_screenshot.png)

## ✨ Features

### Scene-Based Implementation
Experience dynamic gameplay with our scene-based implementation. The game allows you to load and unload scenes seamlessly. Each scene is defined by the memory size needed and four function pointers:
- `on_init`: Initializes the scene.
- `on_event`: Handles events within the scene.
- `on_update`: Updates the state of the scene.
- `on_clear`: Cleans up resources when the scene is unloaded.

### Custom ECS (Entity Component System)
For the bonus part of the project, I have integrated a custom ECS implementation written entirely by myself in C. This system is the backbone of the game's architecture, providing flexible and efficient management of game entities and their components.

#### Why ECS?
The Entity Component System (ECS) architecture is a powerful pattern commonly used in game development. It decouples data and behavior, allowing for greater flexibility and scalability. By using ECS, I can easily manage complex game logic, optimize performance, and facilitate code reuse. The ECS system processes entities through pre-built queries, ensuring O(1) quick access and efficient gameplay mechanics.

### Optimized Collision Detection
Our game features fully generic AABB (Axis-Aligned Bounding Box) optimized collision detection with custom replaceable response handlers. This allows for efficient collision checks and responses between various entities such as:
- Player and walls
- Player and bullets
- Bullets and walls
- Bullets and bullets

### ECS Components and Queries
All gameplay elements are managed using flexible ECS components and processed through pre-built queries for O(1) quick access. This ensures smooth and efficient game mechanics.

### Responsive Camera
The game includes a responsive camera that follows the player, providing an immersive experience as you navigate through the game world.

## 🚀 Getting Started

To get started with the 42 so_long project, follow these steps:

1. **Clone the Repository:**
   ```sh
   git clone --recurse-submodules https://github.com/MykleR/so_long.git
   ```
2. **Navigate to the Project Directory:**
   ```sh
   cd so_long
   ```
3. **Build the Project:**
   ```sh
   make re bonus
   ```
4. **Run the Game:**
   ```sh
   ./so_long_bonus resources/maps/platformer.ber
   ```
