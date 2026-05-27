#include "gs_999.main/class/mesh.hpp"
#include <cmath>
#include <glad/glad.h>
// GLFW
#include <GLFW/glfw3.h>
#include <get_vertex_data.hpp>
#include <iostream>
#include <ostream>

#include <gui/main_gui.hpp>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>
static void framebuffer_size_callback(GLFWwindow *window, int width,
                                      int height);
static void processInput(GLFWwindow *window);

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "Learn Open GL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW Window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -2;
  }

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Initialize gui
  GUI::GuiInit(window);
  Mesh msh;
  msh.insertNew();

  while (!glfwWindowShouldClose(window))
  {
    // input
    processInput(window);

    // rendering code
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    msh.draw();
    // Draw ImGui (populate `meshTriangle` from GUI input)
    GUI::Draw(vertices);
    if (meshTriangle)
    {
      msh.insertNew();
      meshTriangle = false; // consume the one-shot button press
    }

    // buffer
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Cleanup ImGui
  GUI::Destroy();

  msh.destroy();

  glfwTerminate();
  return 0;
}

// framebuffer_size_callback
static void framebuffer_size_callback(GLFWwindow *window, int width,
                                      int height)
{
  glViewport(0, 0, width, height);
}

static void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}
