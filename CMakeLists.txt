cmake_minimum_required (VERSION 3.0)
project (dlx)

find_package(Boost 1.58 COMPONENTS 
    program_options REQUIRED
)

add_executable(dlx 
    Element.cpp 
    main.cpp
    Matrix.cpp 
    Pentominoes.cpp 
    Puzzle.cpp 
    Pyramid.cpp 
    Queens.cpp 
    Soma.cpp 
)

target_compile_options(dlx PRIVATE -std=c++14)
target_link_libraries(dlx Boost::program_options)
