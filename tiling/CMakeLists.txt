add_library(tiling OBJECT
    Pentominoes.cpp
    Pentominoes.hpp
    Pyramid.cpp
    Pyramid.hpp
    Soma.cpp
    Soma.hpp
)

target_include_directories(tiling PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(tiling PUBLIC
    common
)
