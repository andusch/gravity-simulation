# 🌌 OpenGL Gravity Simulator

A real-time 3D N-body gravity simulation built with C++ and OpenGL 3.3. This project simulates the orbital mechanics of a solar system using Newton's Law of Universal Gravitation, featuring a custom Phong lighting engine, a spatial reference grid, and an interactive fly-through camera.

## 🚀 Features

- **N-Body Physics**: Real-time gravitational attraction between all celestial bodies.

- **Complete Solar System**: Includes the Sun and all 8 planets with scaled masses, distances, and unique color profiles.

- **Phong Lighting Model**: Custom fragment shaders calculating Ambient, Diffuse, and Specular (glint) lighting.

- **Orbital Trails**: Persistent visual paths showing the trajectory history of each planet.

- **Fly-through Camera**: 6-DOF (Degrees of Freedom) camera with mouse-look and WASD navigation.

## 🛠️ Technical Stack

- Language: C++ 17
- Graphics: OpenGL 3.3 (Core Profile)
- Windowing: GLFW 3
- GL Loader: GLAD
- Math Library: GLM (OpenGL Mathematics)

## 🎮 Controls

| Key | Action |
| --- | --- |
| W | Move Forward |
| S | Move Backward |
| A | Strafe Left |
| D | Strafe Right |
| Mouse | Look Around |
| Esc | Exit Simulation |

## 🏗️ Project Structure

```
.
├── src/
│   ├── include/
│   │   ├── Body.h
│   │   ├── Color.h
│   │   ├── Constants.h
│   │   ├── Display.h
│   │   ├── Grid.h
│   │   ├── Input.h
│   │   ├── PathUtils.h
│   │   ├── Shader.h
│   │   └── Simulation.h
│   └── shaders/
│       ├── vertex_shader.glsl
│       └── fragment_shader.glsl
├── glad.c
├── main.cpp
├── Makefile
└── README.md

```

## 🧪 Physics Overview

The simulation uses the gravitational force formula:

$$

    F = G \frac{m_1 m_2}{r^2}

$$

Planets are initialized with a Circular Orbit Velocity calculated as:

$$

    v = \sqrt{\frac{G M_{\text{sun}}}{r}}

$$

## ⚙️ Build Instructions

Ensure you have a C++17 compatible compiler, OpenGL 3.3, GLFW, and GLAD installed. Then run:

```bash
git clone https://github.com/andusch/gravity-simulator.git
cd gravity-simulator
make
./gravity-simulator
```
