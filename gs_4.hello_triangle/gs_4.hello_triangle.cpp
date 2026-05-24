#include <glad/glad.h>
// GLFW
#include <GLFW/glfw3.h>
#include <get_vertex_data.hpp>
#include <iostream>
#include <ostream>
static void framebuffer_size_callback(GLFWwindow *window, int width,
                                      int height);
static void processInput(GLFWwindow *window);

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "Learn Open GL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW Window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -2;
  }
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Vertex shader source
  std::string vertexShaderSource =
      get_vertex_data("../gs_4.hello_triangle/vertex.glsl");
  std::string fragmentShaderSource =
      get_vertex_data("../gs_4.hello_triangle/fragment.glsl");

  // Vertex data for a rectangle (4 vertices)
  float vertices[] = {
       0.5f,  0.5f, 0.0f,  // top right    (0)
       0.5f, -0.5f, 0.0f,  // bottom right (1)
      -0.5f, -0.5f, 0.0f,  // bottom left  (2)
      -0.5f,  0.5f, 0.0f   // top left     (3)
  };

  // Indices for two triangles that form a rectangle
  unsigned int indices[] = {
      0, 1, 3,  // first triangle (top-right, bottom-right, top-left)
      1, 2, 3   // second triangle (bottom-right, bottom-left, top-left)
  };

  // Create VAO, VBO, and EBO
  unsigned int VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // Bind VAO first
  glBindVertexArray(VAO);

  // Bind and set VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Bind and set EBO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Configure vertex attributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

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
  }

  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window);

    // rendering code
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw rectangle
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // buffer
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Cleanup
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}

// framebuffer_size_callback
static void framebuffer_size_callback(GLFWwindow *window, int width,
                                      int height) {
  glViewport(0, 0, width, height);
}

static void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}
