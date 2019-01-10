# turba3D
Turba3D:  Generic Multitude Simulation with OpenGL in 3D.

Rendering a multi-agent simulation in real time using OpenGL.

## Compile

A Makefile for Unix like operative systems is included. It can work
with minimal changes for other platforms.

## Usage

Run the executable and press SPACE to start or pause the
simulation. To exit, just press ESCAPE.

Use the keys WASD to navigate and the mouse to orient the camera. The
scroll can zoom the scene.

## Dependencies

C++ compiler, OpenGL headers and libraries, GLFW3, glad (compile it
for your own platform), RVO2 library: Reciprocal Collision Avoidance
for Real-Time Multi-Agent Simulation http://gamma.cs.unc.edu/RVO2/ .

Original code, with the only exception of RVO2 and a few headers from
the tutorial Learn OpenGL ( https://learnopengl.com/ ) and stb_image
in the include directory.

Copyright (c) 2019 Alejandro Aguilar Sierra (asierra@unam.mx)
