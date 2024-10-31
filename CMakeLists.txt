cmake_minimum_required(VERSION 3.18)

project(sotest)

set(CMAKE_C_STANDARD 11)

add_executable(sotest main.c
        lexer.c
        ast.c
        parser.c
        interpreter.c
        interpreter.h)
target_compile_options(sotest PRIVATE -Wall -Wextra)

target_link_libraries(sotest PRIVATE dl ffi)
