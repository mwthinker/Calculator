# calc::Calculator [![CI build](https://github.com/mwthinker/Calculator/actions/workflows/ci.yml/badge.svg)](https://github.com/mwthinker/Calculator/actions/workflows/ci.yml)

A Calculator api created in C++. The goal for the api is to take a mathematical 
expression and calculate the value at runtime.

The project uses C++20 and the standard c++ library.

## Building project locally
CMake and vcpkg is needed to run locally.

Inside the project folder, e.g.
```bash
cmake --preset=unix -B build -DCalculator_Test=1; cmake --build build; ctest --test-dir build/Calculator_Test
./build/Signal_Example/Signal_Example
```

## Open source
The project is under the MIT license (see LICENSE.txt).
