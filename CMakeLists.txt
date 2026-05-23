cmake_minimum_required(VERSION 3.10)

project(ogl VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Target file can be set from command line with -DTARGET=path/to/file.cpp
if(NOT DEFINED TARGET)
    message(FATAL_ERROR "TARGET is not defined. Use: cmake -DTARGET=path/to/file.cpp")
endif()

set(SOURCES
    ${TARGET}
    glad/src/glad.c
)

find_package(glfw3 REQUIRED)

add_executable(my_app ${SOURCES})

target_include_directories(my_app PRIVATE
    glad/include
)

# Compile flags
target_compile_options(my_app PRIVATE -Wall -Wextra)

# Link libraries
target_link_libraries(my_app PRIVATE
    glfw
    GL
    X11
    pthread
    Xrandr
    Xi
)
