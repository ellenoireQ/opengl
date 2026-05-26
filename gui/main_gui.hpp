#ifndef MAIN_GUI_HPP
#define MAIN_GUI_HPP
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>

class GUI
{
public:
  //
  // Initialize Graphical User Interface
  //
  static void GuiInit(GLFWwindow *window);

  static void Draw(float matrix[]);

  static void app(float matrix[]);

  static void Destroy();

  // Get the size array values
  static const float *getSizeArray();

  // Get the color array values
  static const float *getColorArray();
};

#endif // MAIN_GUI_HPP
