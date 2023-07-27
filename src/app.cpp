#include "app.h"

ImFont* default_font;

void app_init() {
  default_font = helper_load_resource_font(100, 20.0f);
}

void app() {
  ImGui::ShowDemoWindow(NULL);
}

