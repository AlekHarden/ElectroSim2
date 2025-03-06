# ElectroSim 2

## Description

  2D simulation of electric particles in a vacuum.
  
  ![alt text][logo]

  [logo]: <https://github.com/alek145/ElectroSim2/blob/master/res/icon/Demo.PNG> "Logo Title Text 2"
  
## Install Dependencies

- Install CMake

### Linux

  `sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev`

### Windows

Install [MSYS2](https://www.msys2.org/)

Within MSYS2 terminal, install the following :

- **GLEW** The OpenGL Extension Wrangler Library

  `pacman -S mingw-w64-x86_64-glew`

- **GLFW** A free, open source, portable framework for OpenGL application development

  `pacman -S mingw-w64-x86_64-glfw`

- **GLM** C++ mathematics library for 3D software based on the OpenGL Shading Language (GLSL) specification

  `pacman -S mingw-w64-ucrt-x86_64-glm`

## Build

### Linux

### Windows

In the root of the repository:

 1. `mkdir build`
 2. `cd build`
 3. `cmake ..`
 4. `cmake --build .`
