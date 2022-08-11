cmake_minimum_required(VERSION 3.09)

set(CMAKE_CXX_STANDARD 17)

#create project
project(qcd VERSION 1.0)

include_directories(include)
include_directories(cpp-sqlitelib)
include_directories(cpp-sqlitelib/test)


add_executable(qcd 
				src/main.cpp
				cpp-sqlitelib/test/sqlite3.c)
