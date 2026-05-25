#include "get_vertex_data.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

#ifndef OGL_SOURCE_DIR
#define OGL_SOURCE_DIR ""
#endif

std::string get_vertex_data(const char* filepath) {
    std::filesystem::path filePath(filepath);
    if (filePath.is_relative() && OGL_SOURCE_DIR[0] != '\0') {
        filePath = std::filesystem::path(OGL_SOURCE_DIR) / filePath;
    }

    std::ifstream file(filePath);
    
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath.string() << std::endl;
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    
    return buffer.str();
}
