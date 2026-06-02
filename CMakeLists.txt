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
    gs_999.main/class/mesh.cpp

    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_tables.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

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
    gs_999.main/class

)

if(WIN32)
    target_include_directories(my_app PRIVATE
        glfw-3.4.bin/include/
    )
    target_link_libraries(my_app PRIVATE
        OpenGL::GL
        ${CMAKE_CURRENT_SOURCE_DIR}/glfw-3.4.bin/lib-mingw-w64/libglfw3.a
    )
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW3 REQUIRED IMPORTED_TARGET glfw3)
    target_link_libraries(my_app PRIVATE
        OpenGL::GL
        PkgConfig::GLFW3
        ${CMAKE_DL_LIBS}
    )
endif()
