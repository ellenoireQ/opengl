#include "main_gui.hpp"

static float myFloat = 0.5f;

void app() {
  ImGui::Text("Hello, world %d", 123);
  if (ImGui::Button("Save")) {
    // Button clicked
  }
  ImGui::SliderFloat("float", &myFloat, 0.0f, 1.0f);
}
