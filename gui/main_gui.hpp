#ifndef MAIN_GUI_HPP
#define MAIN_GUI_HPP
#include <gs_999.main/class/global.hpp>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>
#include <string>
#include <vector>

inline float xLoc = 0.0f, yLoc = 0.0f;
inline std::vector<std::string> meshOptions;
inline int selectedMeshIndex = 0;

class GUI {
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

  // Get the location values
  static float getXLoc() { return xLoc; }
  static float getYLoc() { return yLoc; }

  static void setLocation(float x, float y);
  static void setSizeArray(const float sizeData[9]);
  static void setColorArray(const float colorData[3]);

  static int getSelectedMeshIndex() { return selectedMeshIndex; }

  static void RegisterMeshOption(const std::string &label);
};

#endif // MAIN_GUI_HPP
