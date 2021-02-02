cmake_minimum_required(VERSION 3.13)
project(dlx)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost 1.58 COMPONENTS 
    program_options REQUIRED
)

add_subdirectory(common)
add_subdirectory(queens)
add_subdirectory(sudoku)
add_subdirectory(tiling)

add_executable(dlx
    main.cpp
)

target_link_libraries(dlx 
    common
    queens
    sudoku
    tiling
    Boost::program_options
)
