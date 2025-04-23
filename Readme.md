# Mandelbrot GPU Explorer

A GPU-accelerated Mandelbrot set explorer that allows interactive navigation with zoom and drag capabilities.

## Features

- Real-time rendering using OpenGL fragment shaders
- Zoom by clicking on interesting areas
- Pan by right-click dragging
- Adaptive iteration counts for deep zooms
- Enhanced coloring with orbit traps
- Smooth transition coloring
- Screenshot capability

## Controls

- **Left-click**: Zoom in at cursor position
- **Right-click and drag**: Pan around the fractal
- **Mouse wheel**: Zoom in/out
- **P key**: Save a screenshot (saved as screenshot.ppm)
- **Escape key**: Exit

## Building

### Requirements

- CMake 3.16+
- SDL2
- OpenGL development libraries
- GLEW
- Boost (program_options)
- GLM

### Build Instructions

```bash
mkdir -p build
cd build
cmake ..
make
```

Or simply run the provided build script:

```bash
./r
```

## Running

```bash
./build/mandelbrot_gpu
```

### Command-line options

```
Options:
  --help                  Show help
  --width arg (=800)      Window width
  --height arg (=600)     Window height
  --scale arg (=2.5)      Initial zoom scale
  --center-x arg (=-0.75) Center X coordinate
  --center-y arg (=0)     Center Y coordinate
```

For example, to start with a specific center and zoom level:

```bash
./build/mandelbrot_gpu --center-x=-0.235125 --center-y=0.827215 --scale=0.00004
```

## Project Structure

- `src/`: Source files
  - `main.cpp`: Entry point
  - `app.cpp/app.h`: Main application class
  - `shader.cpp/shader.hpp`: Shader loading and compilation
  - `config.cpp/config.hpp`: Command-line parsing
- `shaders/`: GLSL shader files
  - `mandelbrot.frag`: Fragment shader for Mandelbrot set rendering
- `include/`: Header files

## Technical Details

This application uses:

- OpenGL for GPU-accelerated rendering
- Fragment shaders for per-pixel calculation of the Mandelbrot set
- SDL2 for window management and input handling
- Boost for command-line parsing
- GLM for vector mathematics

The Mandelbrot set is rendered using adaptive iteration counts that increase with deeper zoom levels, providing more detail when needed while maintaining performance. The application also uses smooth coloring and orbit traps to enhance visual quality.
