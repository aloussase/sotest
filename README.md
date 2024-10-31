# sotest

A tiny interpreter for loading and testing shared libraries.

## Installation

You'll need a C compiler and CMake installed.

```
git clone https://github.com/aloussase/sotest
cd sotest
cmake -B build -S .
cmake --build build
```

## Usage

To spawn a REPL:

```
./build/sotest
```

To evaluate the contents of a file:

```
./build/sotest/<filename>
```

## Demo
