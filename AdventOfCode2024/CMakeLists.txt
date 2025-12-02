cmake_minimum_required(VERSION 3.30)
project(AdventOfCode2024)

set(CMAKE_CXX_STANDARD 20)

# Add each day's task as a separate executable
# add_executable(day01 day01.cpp)

# Automatically add all dayXX.cpp files as executables
file(GLOB TASKS "day*.cpp")
foreach(task ${TASKS})
    get_filename_component(name ${task} NAME_WE)
    add_executable(${name} ${task}
            utils/input_reader.cpp
            utils/input_reader.h
    )
endforeach()
