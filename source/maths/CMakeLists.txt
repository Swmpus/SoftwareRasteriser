add_library(Maths
    maths.hpp
    maths.cpp
)
target_include_directories(Maths
	PUBLIC
    	${CMAKE_CURRENT_LIST_DIR}
)
