# COMP371 — Assignment 2

An introductory OpenGL program for COMP371 (Introduction to Computer Graphics) at Concordia University. It opens a window and renders a colored 3D pyramid using modern OpenGL (core profile 3.3) with vertex/fragment shaders, a VAO/VBO/EBO, and a GLM transformation matrix.

## Dependencies

- **GLEW** — OpenGL extension loader (`glew-2.3.1/`)
- **GLFW 3.4** — windowing and input (`glfw-3.4.bin.WIN64/`)
- **GLM** — math library, header-only (`glm-master/`)
- An OpenGL 3.3 capable GPU / driver
- A C++ compiler (MSVC on Windows, or `g++` on Linux)

The GLEW and GLM sources plus prebuilt Windows GLFW binaries are vendored in the repo, so no external download is required on Windows.

## Project layout

```
OpenGLIntro/
  OpenGLIntro.slnx              Visual Studio solution
  OpenGLIntro/
    OpenGLIntro.cpp             main program (window + pyramid)
    OpenGLIntro.vcxproj         Visual Studio project
    Makefile                    Linux build
glew-2.3.1/                     GLEW source
glfw-3.4.bin.WIN64/             GLFW prebuilt Windows binaries
glm-master/                     GLM headers
```

## Build & run

### Windows (Visual Studio)

1. Open `OpenGLIntro/OpenGLIntro.slnx` in Visual Studio (2017 or newer).
2. Select the **x64** platform configuration.
3. Build and run with **F5** (Debug) or **Ctrl+F5** (Release).

The project is already configured to find the vendored GLEW, GLFW, and GLM. Make sure `glew32.dll` is on the PATH or next to the executable at runtime.

### Linux (Makefile)

Install the development packages, then build with the provided Makefile:

```bash
# Debian/Ubuntu
sudo apt install build-essential libglew-dev libglfw3-dev libglm-dev pkg-config

cd OpenGLIntro/OpenGLIntro
make        # builds the ./a2 executable
make run    # builds and runs
make clean  # removes the executable
```

The Makefile uses `pkg-config` to locate GLEW and GLFW and links against system OpenGL (`-lGL`).

## Controls

- **Esc** — close the window
- **W** — translate the shape upward

## Assignment brief

See `Assignment 2-Summer 2026.pdf` for the full requirements.
