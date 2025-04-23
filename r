#!/bin/bash
mkdir -p build
cd build && cmake ..  make && ./mandelbrot_gpu $@
