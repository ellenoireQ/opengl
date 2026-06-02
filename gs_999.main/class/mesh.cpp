#include "mesh.hpp"
#include "gui/main_gui.hpp"
#include <string>
#include <utils/get_vertex_data.hpp>

MeshContainer Mesh::container{};
MeshStructure Mesh::block{};
static int lastSyncedMeshIndex = -1;

void updateVerticesFromGui(float targetVertices[]) {
  const float *currentSize = GUI::getSizeArray();
  const float *currentColor = GUI::getColorArray();

  targetVertices[0] = currentSize[0];
  targetVertices[1] = currentSize[1];
  targetVertices[2] = currentSize[2];
  targetVertices[6] = currentSize[3];
  targetVertices[7] = currentSize[4];
  targetVertices[8] = currentSize[5];
  targetVertices[12] = currentSize[6];
  targetVertices[13] = currentSize[7];
  targetVertices[14] = currentSize[8];

  targetVertices[3] = currentColor[0];
  targetVertices[10] = currentColor[1];
  targetVertices[17] = currentColor[2];
}

static void syncGuiFromMesh(const MeshStructure &msh) {
  const float sizeData[9] = {msh.vertices[0],  msh.vertices[1],  msh.vertices[2],
                             msh.vertices[6],  msh.vertices[7],  msh.vertices[8],
                             msh.vertices[12], msh.vertices[13], msh.vertices[14]};
  const float colorData[3] = {msh.vertices[3], msh.vertices[10], msh.vertices[17]};

  GUI::setSizeArray(sizeData);
  GUI::setColorArray(colorData);
  GUI::setLocation(msh.loc.x, msh.loc.y);
}

bool Mesh::init(MeshStructure &msh) {
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
  if (!success) {
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
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
    return false;
  }

  msh.shaderProgram = glCreateProgram();
  glAttachShader(msh.shaderProgram, vertexShader);
  glAttachShader(msh.shaderProgram, fragmentShader);
  glLinkProgram(msh.shaderProgram);

  msh.offsetLoc = glGetUniformLocation(msh.shaderProgram, "offset");

  glGetProgramiv(msh.shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
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

void Mesh::destroy() {
  for (auto &i : container.mesh_block) {
    glDeleteVertexArrays(1, &i.VAO);
    glDeleteBuffers(1, &i.VBO);
    glDeleteBuffers(1, &i.EBO);
    glDeleteProgram(i.shaderProgram);
  }
}

void Mesh::insertNew() {
  const int meshId = static_cast<int>(container.mesh_block.size()) + 1;
  block = MeshStructure{
      //
      .name = std::to_string(meshId),
      .width = 400,
      .height = 400,
      .fragment = get_vertex_data("gs_999.main/fragment.glsl"),
      .vertex = get_vertex_data("gs_999.main/vertex.glsl"),
      .VAO = 0,
      .VBO = 0,
      .EBO = 0,
  };

  // Copy current vertices data to this mesh
  updateVerticesFromGui(block.vertices);
  block.loc = {GUI::getXLoc(), GUI::getYLoc()};

  if (init(block)) {
    container.mesh_block.push_back(block);
    std::cout << "Mesh spawned! Total meshes: " << container.mesh_block.size()
              << std::endl;
    GUI::RegisterMeshOption(block.name);
  }
}

void Mesh::draw() {
  const int selectedMeshIndex = GUI::getSelectedMeshIndex();

  for (size_t i = 0; i < container.mesh_block.size(); i++) {
    auto &msh = container.mesh_block[i];

    if (static_cast<int>(i) == selectedMeshIndex) {
      if (lastSyncedMeshIndex != selectedMeshIndex) {
        syncGuiFromMesh(msh);
        lastSyncedMeshIndex = selectedMeshIndex;
      }
      msh.loc.x = GUI::getXLoc();
      msh.loc.y = GUI::getYLoc();
      updateVerticesFromGui(msh.vertices);

      glBindBuffer(GL_ARRAY_BUFFER, msh.VBO);
      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(msh.vertices), msh.vertices);
    }

    glUseProgram(msh.shaderProgram);

    glUniform2f(msh.offsetLoc, msh.loc.x, msh.loc.y);

    // Draw triangle
    glBindVertexArray(msh.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }
}
