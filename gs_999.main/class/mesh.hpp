#ifndef MESH_HPP
#define MESH_HPP
#include "get_vertex_data.hpp"
#include "gui/main_gui.hpp"
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <vector>

static const float *sizeArray = GUI::getSizeArray();
static float vertices[] = {
    // positions                              // colors
    sizeArray[0], sizeArray[1], sizeArray[2], 1.0f, 0.0f, 0.0f, // bottom right
    sizeArray[3], sizeArray[4], sizeArray[5], 0.0f, 1.0f, 0.0f, // bottom left
    sizeArray[6], sizeArray[7], sizeArray[8], 0.0f, 0.0f, 1.0f  // top
};

void updateVerticesFromGui();

struct MeshStructure
{
  const char *name;
  int width;
  int height;

  std::string fragment;
  std::string vertex;

  unsigned int VAO, VBO, EBO;
  unsigned int shaderProgram;
};

struct MeshContainer
{
  std::vector<MeshStructure> mesh_block;
};

class Mesh
{
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
