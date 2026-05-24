#include "get_vertex_data.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

std::string get_vertex_data(const char* filepath) {
    std::ifstream file(filepath);
    
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    
    return buffer.str();
}
