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
    utils/get_vertex_data.cpp
    utils/utility.cpp
    gui/main_gui.cpp

    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_tables.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

set(GLFW_TARGET glfw)
find_package(glfw3 QUIET)

if(NOT TARGET glfw AND NOT TARGET glfw3)
    set(GLFW_BUNDLED_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui/examples/libs/glfw)

    if(EXISTS ${GLFW_BUNDLED_DIR}/include/GLFW/glfw3.h)
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(GLFW_BUNDLED_LIB_DIR ${GLFW_BUNDLED_DIR}/lib-vc2010-64)
        else()
            set(GLFW_BUNDLED_LIB_DIR ${GLFW_BUNDLED_DIR}/lib-vc2010-32)
        endif()

        add_library(glfw STATIC IMPORTED)
        set_target_properties(glfw PROPERTIES
            IMPORTED_LOCATION ${GLFW_BUNDLED_LIB_DIR}/glfw3.lib
            INTERFACE_INCLUDE_DIRECTORIES ${GLFW_BUNDLED_DIR}/include
        )
    else()
        message(FATAL_ERROR "GLFW not found. Install glfw3 or restore imgui/examples/libs/glfw.")
    endif()
elseif(TARGET glfw3)
    set(GLFW_TARGET glfw3)
endif()

# OpenGL
find_package(OpenGL REQUIRED)

add_executable(my_app ${SOURCES})

target_compile_definitions(my_app PRIVATE
    OGL_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

target_include_directories(my_app PRIVATE
    glad/include
    utils/
    ./
    ./imgui
    ./imgui/backends
)

# Compile flags
if(MSVC)
    target_compile_options(my_app PRIVATE /W4)
else()
    target_compile_options(my_app PRIVATE -Wall -Wextra)
endif()

# Link libraries
if(WIN32)
    target_link_libraries(my_app PRIVATE
        ${GLFW_TARGET}
        OpenGL::GL
    )
elseif(APPLE)
    target_link_libraries(my_app PRIVATE
        ${GLFW_TARGET}
        OpenGL::GL
    )
else()
    target_link_libraries(my_app PRIVATE
        ${GLFW_TARGET}
        OpenGL::GL
        X11
        pthread
        Xrandr
        Xi
    )
endif()
