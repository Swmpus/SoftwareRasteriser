cmake_minimum_required(VERSION 4.2.1)

project(SoftwareRasteriser VERSION 0.0 LANGUAGES CXX)

add_executable(main
	main.cpp
)
add_subdirectory(maths)

target_link_libraries(main
	Maths
)
