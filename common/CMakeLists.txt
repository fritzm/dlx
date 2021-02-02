add_library(common OBJECT
    Element.cpp
    Element.hpp
    Matrix.cpp
    Matrix.hpp
    Puzzle.cpp
    Puzzle.hpp
)

target_include_directories(common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

