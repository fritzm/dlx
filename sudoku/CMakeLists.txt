add_library(sudoku OBJECT
    Sudoku.cpp
    Sudoku.hpp
)

target_include_directories(sudoku PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(sudoku PUBLIC
    common
)
