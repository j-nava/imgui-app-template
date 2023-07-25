#pragma once

#include <string>
#include "imgui.h"
#include <d3d11.h>

extern ID3D11Device* g_pd3dDevice;
struct Texture {
  ID3D11ShaderResourceView* t;
  int width = 0;
  int height = 0;
};
typedef struct Texture Texture;

struct Resource {
  void* ptr;
  size_t size_bytes;
};
typedef struct Resource Resource;

void helper_init();

bool helper_load_texture_from_file(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
bool helper_load_texture_from_memory(const unsigned char* buffer, int len, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
ImFont* helper_load_resource_font(const int resource_id, const float size_pixels);
Texture helper_load_resource_texture(const int resource_id);

void helper_text_centered(std::string text);

