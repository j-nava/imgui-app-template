#pragma once

#include "imgui.h"
#include "imgui_internal.h"

#include <string>

void helper_init(); 
//export bool helper_load_texture_from_file(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height) {
//export bool helper_load_texture_from_memory(const unsigned char* buffer, int len, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height) {
void helper_text_centered(std::string text);
void helper_viewport_dockspace();
void helper_viewport_dockspace_left_right(std::string left, std::string right, float ratio);
void helper_begin_maximized();
