module;

#define STB_IMAGE_IMPLEMENTATION
#include <string>
#include "stb_image.h"
#include "imgui.h"
#include <d3d11.h>

export module helper;

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

export void helper_init() {
  ImGui::GetIO().IniFilename = "app.ini";
}

bool LoadTexture(unsigned char* image_data, int image_width, int image_height, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
  // Create texture
  D3D11_TEXTURE2D_DESC desc;
  ZeroMemory(&desc, sizeof(desc));
  desc.Width = image_width;
  desc.Height = image_height;
  desc.MipLevels = 1;
  desc.ArraySize = 1;
  desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  desc.SampleDesc.Count = 1;
  desc.Usage = D3D11_USAGE_DEFAULT;
  desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  desc.CPUAccessFlags = 0;

  ID3D11Texture2D* pTexture = NULL;
  D3D11_SUBRESOURCE_DATA subResource;
  subResource.pSysMem = image_data;
  subResource.SysMemPitch = desc.Width * 4;
  subResource.SysMemSlicePitch = 0;
  g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

  if (pTexture) {
    // Create texture view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
    pTexture->Release();
  }

  *out_width = image_width;
  *out_height = image_height;

  return true;
}
export bool helper_load_texture_from_file(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height) {
  int image_width = 0;
  int image_height = 0;
  unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
  if (image_data == NULL)
    return false;
  bool res = LoadTexture(image_data, image_width, image_height, out_srv, out_width, out_height);
  stbi_image_free(image_data);
  return res;
}
export bool helper_load_texture_from_memory(const unsigned char* buffer, int len, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height) {
  int image_width = 0;
  int image_height = 0;
  unsigned char* image_data = stbi_load_from_memory(buffer, len, &image_width, &image_height, NULL, 4);
  if (image_data == NULL)
    return false;
  bool res = LoadTexture(image_data, image_width, image_height, out_srv, out_width, out_height);
  stbi_image_free(image_data);
  return res;
}

export void helper_text_centered(std::string text) {
  float win_width = ImGui::GetWindowSize().x;
  float text_width = ImGui::CalcTextSize(text.c_str()).x;

  // calculate the indentation that centers the text on one line, relative
  // to window left, regardless of the `ImGuiStyleVar_WindowPadding` value
  float text_indentation = (win_width - text_width) * 0.5f;

  // if text is too long to be drawn on one line, `text_indentation` can
  // become too small or even negative, so we check a minimum indentation
  float min_indentation = 20.0f;
  if (text_indentation <= min_indentation) {
    text_indentation = min_indentation;
  }

  ImGui::SameLine(text_indentation);
  ImGui::PushTextWrapPos(win_width - text_indentation);
  ImGui::TextWrapped("%s", text.c_str());
  ImGui::PopTextWrapPos();
  ImGui::Text("");
}

Resource helper_load_resource(const int resource_id) {
  HRSRC hResource = FindResourceEx(nullptr, L"DATA", MAKEINTRESOURCE(resource_id), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL));
  Resource r;
  bool success = false;

  if (hResource) {
    HGLOBAL hMemory = LoadResource(nullptr, hResource);
    size_t size_bytes = SizeofResource(nullptr, hResource);
    if (hMemory) {
      void* ptr = LockResource(hMemory);
      r.ptr = ptr;
      r.size_bytes = size_bytes;
      success = true;
    }
  }
  IM_ASSERT(success);

  return r;
}

ImFont* helper_load_resource_font(const int resource_id, const float size_pixels) {
  Resource r = helper_load_resource(resource_id);

  ImGuiIO& io = ImGui::GetIO();
  ImFontConfig font_config;
  font_config.FontDataOwnedByAtlas = false;
  ImFont* font = io.Fonts->AddFontFromMemoryTTF(r.ptr, (int)r.size_bytes, size_pixels, &font_config);

  return font;
}

Texture helper_load_resource_texture(const int resource_id) {
  Resource r = helper_load_resource(resource_id);

  Texture tex;
  bool ret = helper_load_texture_from_memory((const unsigned char*)r.ptr, (int)r.size_bytes, &tex.t, &tex.width, &tex.height);
  IM_ASSERT(ret);
  return tex;
}

