#include "main_gui.hpp"
#include "imgui.h"
#include <cstdio>
#include <utils/utility.hpp>

static float myFloat = 0.5f;
static float size[9] = {0.5f, -0.5f, 0.0f,  // vertex 1: x, y, z
                        -0.5f, -0.5f, 0.0f, // vertex 2: x, y, z  
                        0.0f, 0.5f, 0.0f};  // vertex 3: x, y, z

void GUI::app(float matrix[]) {
  static float color[3] = {0.2f, 0.6f, 1.0f};

  ImGui::Text("OpenGL Control Panel");
  ImGui::Separator();

  if (ImGui::Button("Save Config")) {
    //
  }

  ImGui::Spacing();
  setColorByMatrix(matrix, color[0], color[1], color[2]);
  setSize(matrix, size);
  if (ImGui::BeginTable("size_table", 2, ImGuiTableFlags_SizingStretchSame)) {
    for (int i = 0; i < 9; i++) {
      ImGui::TableNextColumn();
      ImGui::Text("%d", i + 1);

      ImGui::TableNextColumn();
      ImGui::PushItemWidth(-1);
      char label[16];
      snprintf(label, sizeof(label), "##Size%d", i);
      ImGui::SliderFloat(label, &size[i], 0.0f, 1.0f);
      ImGui::PopItemWidth();

      if (i < 8)
        ImGui::TableNextRow();
    }
    ImGui::EndTable();
  }

  ImGui::Spacing();

  if (ImGui::BeginTable("rgb_table", 2, ImGuiTableFlags_SizingStretchSame)) {
    // ===== R =====
    ImGui::TableNextColumn();
    ImGui::Text("Red");

    ImGui::TableNextColumn();
    ImGui::PushItemWidth(-1);
    ImGui::SliderFloat("##R", &color[0], 0.0f, 1.0f);
    ImGui::PopItemWidth();

    // ===== G =====
    ImGui::TableNextRow();

    ImGui::TableNextColumn();
    ImGui::Text("Green");

    ImGui::TableNextColumn();
    ImGui::PushItemWidth(-1);
    ImGui::SliderFloat("##G", &color[1], 0.0f, 1.0f);
    ImGui::PopItemWidth();

    // ===== B =====
    ImGui::TableNextRow();

    ImGui::TableNextColumn();
    ImGui::Text("Blue");

    ImGui::TableNextColumn();
    ImGui::PushItemWidth(-1);
    ImGui::SliderFloat("##B", &color[2], 0.0f, 1.0f);
    ImGui::PopItemWidth();

    ImGui::EndTable();
  }

  ImGui::Spacing();

  // Preview color
  ImGui::Text("Preview");
  ImGui::ColorButton("##preview", ImVec4(color[0], color[1], color[2], 1.0f),
                     ImGuiColorEditFlags_NoTooltip, ImVec2(200, 50));
}

void GUI::GuiInit(GLFWwindow *window) {
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");
}

void GUI::Draw(float matrix[]) {
  // Start new ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // Create a window
  ImGui::Begin("Control Panel");
  app(matrix);
  ImGui::End();

  // Render ImGui
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::Destroy() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

const float* GUI::getSizeArray() {
  return size;
}
