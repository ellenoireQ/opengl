#include "mesh.hpp"
#include <utils/get_vertex_data.hpp>

MeshContainer Mesh::container{};
MeshStructure Mesh::block{};

void updateVerticesFromGui()
{
  const float *currentSize = GUI::getSizeArray();
  const float *currentColor = GUI::getColorArray();

  vertices[0] = currentSize[0];
  vertices[1] = currentSize[1];
  vertices[2] = currentSize[2];
  vertices[6] = currentSize[3];
  vertices[7] = currentSize[4];
  vertices[8] = currentSize[5];
  vertices[12] = currentSize[6];
  vertices[13] = currentSize[7];
  vertices[14] = currentSize[8];

  vertices[3] = currentColor[0];
  vertices[10] = currentColor[1];
  vertices[17] = currentColor[2];
}

bool Mesh::init(MeshStructure &msh)
{
  std::string vertexShaderSource = msh.vertex;
  std::string fragmentShaderSource = msh.fragment;

  glGenVertexArrays(1, &msh.VAO);
  glGenBuffers(1, &msh.VBO);
  glGenBuffers(1, &msh.EBO);

  // Bind VAO first
  glBindVertexArray(msh.VAO);

  // Bind and set VBO
  glBindBuffer(GL_ARRAY_BUFFER, msh.VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(msh.vertices), msh.vertices,
               GL_STATIC_DRAW);

  // Configure vertex attributes
  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Unbind (optional, for safety)
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  // Note: Don't unbind EBO while VAO is active, it's stored in VAO

  // Create and compile vertex shader
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  const char *vertexShaderSourceStr = vertexShaderSource.c_str();
  glShaderSource(vertexShader, 1, &vertexShaderSourceStr, NULL);
  glCompileShader(vertexShader);

  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
    return false;
  }

  // Create and compile fragment shader
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  const char *fragmentShaderStr = fragmentShaderSource.c_str();
  glShaderSource(fragmentShader, 1, &fragmentShaderStr, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
    return false;
  }

  msh.shaderProgram = glCreateProgram();
  glAttachShader(msh.shaderProgram, vertexShader);
  glAttachShader(msh.shaderProgram, fragmentShader);
  glLinkProgram(msh.shaderProgram);

  glGetProgramiv(msh.shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(msh.shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
    return false;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  int nrAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes
            << std::endl;

  return true;
}

void Mesh::destroy()
{
  for (auto &i : container.mesh_block)
  {
    glDeleteVertexArrays(1, &i.VAO);
    glDeleteBuffers(1, &i.VBO);
    glDeleteBuffers(1, &i.EBO);
    glDeleteProgram(i.shaderProgram);
  }
}

void Mesh::insertNew()
{
  block = MeshStructure{
      //
      .name = "123",
      .width = 400,
      .height = 400,
      .fragment = get_vertex_data("gs_999.main/fragment.glsl"),
      .vertex = get_vertex_data("gs_999.main/vertex.glsl"),
      .VAO = 0,
      .VBO = 0,
      .EBO = 0,
  };

  // Copy current vertices data to this mesh
  updateVerticesFromGui();
  for (int i = 0; i < 18; i++)
  {
    block.vertices[i] = vertices[i];
  }

  if (init(block))
  {
    container.mesh_block.push_back(block);
    std::cout << "Mesh spawned! Total meshes: " << container.mesh_block.size()
              << std::endl;
  }
}

void Mesh::draw()
{
  updateVerticesFromGui();

  for (size_t i = 0; i < container.mesh_block.size(); i++)
  {
    auto &msh = container.mesh_block[i];

    // Draw triangle
    glUseProgram(msh.shaderProgram);

    if (i == 0)
    {
      glBindBuffer(GL_ARRAY_BUFFER, msh.VBO);
      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    }

    glBindVertexArray(msh.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }
}
