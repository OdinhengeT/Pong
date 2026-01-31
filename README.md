# == Pong ==

This repository contains a from scratch implementation of the video game classic Pong, written in C++ using SDL3 and built with CMake. The previous implementation used the win32 library and was build through regular make, but with SDL3 it is hopefully fully cross-platform. This is an educational project and not a feature complete game. Features such as audio and music are still missing, and there are quite a few implementation quirks elsewhere.

# Build

Clone the Repository:

> git clone git@github.com:OdinhengeT/Pong.git

Initialize CMake:

> cmake -S . -B build

Build with CMake:

> cmake --build build
