cmake_minimum_required(VERSION 4.2.1)

project(SoftwareRasteriser VERSION 0.0 LANGUAGES CXX)

add_executable(main
	main.cpp
)
add_subdirectory(maths)
add_subdirectory(io)

target_link_libraries(main
	Maths
	IO
)
