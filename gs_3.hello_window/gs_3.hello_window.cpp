#include <glad/glad.h>
// GLFW
#include <GLFW/glfw3.h>
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

  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window);

    // rendering code
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // buffer
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

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
