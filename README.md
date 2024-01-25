# calc::Calculator [![CI build](https://github.com/mwthinker/Calculator/actions/workflows/ci.yml/badge.svg)](https://github.com/mwthinker/Calculator/actions/workflows/ci.yml) [![codecov](https://codecov.io/gh/mwthinker/Calculator/graph/badge.svg?token=6FBWEIWGD6)](https://codecov.io/gh/mwthinker/Calculator) [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

A Calculator api created in C++. The goal for the api is to take a mathematical 
expression and calculate the value at runtime.

The project uses C++20 and the C++ Standard Library.

## Code example
```cpp
#include <iostream>
#include <string>

#include <calc/calculator.h>

int main() {
    calc::Calculator calculator;

    std::cout << "1 - (-(2^2)) - 1 = " << calculator.excecute("1 - (-(2^2)) - 1") << "\n";
    
    calculator.addVariable("pi", 3.1416f);

    std::cout << "2^2 * pi" << calculator.excecute("2^2 * pi") << "\n";

    return 0;
}
```
Output:
```
1 - (-(2^2)) - 1 = 4.0
2^2 * pi = 12.5664
```
For more example code see [Calculator_Benchmark](https://github.com/mwthinker/Calculator/blob/master/Calculator_Benchmark/src/speedtest.cpp) or [Calculator_Test](https://github.com/mwthinker/Calculator/blob/master/Calculator_Test/src/tests.cpp).

## Building project locally
[CMake](https://cmake.org/) and [vcpkg](https://github.com/microsoft/vcpkg.git) can be used to run the repository locally.

Inside the project folder, e.g.
```bash
cmake --preset=unix -B build -DCalculator_Test=1; cmake --build build; ctest --test-dir build/Calculator_Test
./build/Signal_Example/Signal_Example
```

## Open source
The project is under the MIT license (see LICENSE.txt).
