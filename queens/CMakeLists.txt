add_library(queens OBJECT
    Queens.cpp
    Queens.hpp
)

target_include_directories(queens PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(queens PUBLIC
    common
)
