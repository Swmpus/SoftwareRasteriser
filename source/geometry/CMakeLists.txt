add_library(Geometry
    geometry.hpp
    geometry.cpp
)
target_link_libraries(Geometry
    PRIVATE Maths
)
target_include_directories(Geometry
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)
