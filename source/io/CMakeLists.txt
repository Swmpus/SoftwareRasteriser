add_library(IO
    io.hpp
    io.cpp
)
target_include_directories(IO
	PUBLIC
    	${CMAKE_CURRENT_LIST_DIR}
)
