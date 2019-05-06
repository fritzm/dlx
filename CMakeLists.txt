cmake_minimum_required (VERSION 3.0)
project (psolve)
add_executable(psolve Header.cpp Element.cpp Matrix.cpp psolve.cpp)
target_compile_options(psolve PRIVATE -std=c++14)