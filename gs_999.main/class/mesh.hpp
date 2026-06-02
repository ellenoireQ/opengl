#ifndef MESH_HPP
#define MESH_HPP
#include "get_vertex_data.hpp"
#include "gui/main_gui.hpp"
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <vector>

static float vertices[] = {
    // positions                              // colors
    0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
    0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // top
};

void updateVerticesFromGui();

struct Coord {
  float x;
  float y;
};
struct MeshStructure {
  std::string name;
  int width;
  int height;

  std::string fragment;
  std::string vertex;

  unsigned int VAO, VBO, EBO;
  unsigned int shaderProgram;
  GLint offsetLoc;

  // Each mesh stores its own vertex data
  float vertices[18]; // 3 vertices * 6 floats (3 pos + 3 color)

  struct Coord loc;
};

struct MeshContainer {
  std::vector<MeshStructure> mesh_block;
};

class Mesh {
public:
  bool init(MeshStructure &ms);
  void destroy();
  void draw();

  static struct MeshContainer container;
  static struct MeshStructure block;

  // Expand the size
  // width++
  // height++
  void tapExpand();
  void insertNew();
};
//
#endif // MESH_HPP
