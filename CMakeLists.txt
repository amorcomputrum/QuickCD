cmake_minimum_required(VERSION 3.09)

#create project
project(QuickCD VERSION 0.0.1)

include_directories(include)

add_executable(QuickCD 
				src/main.cpp)

#target_link_libraries(QuickCD glfw)
