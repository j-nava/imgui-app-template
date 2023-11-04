cmake_minimum_required(VERSION 3.28)
project(imgui_app_template LANGUAGES C CXX)

include(FetchContent)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

# -----------------------------------------------------------------------------------------------------

FetchContent_Declare(
  imgui 
  GIT_REPOSITORY https://github.com/ocornut/imgui
  GIT_TAG        208b8242bc8dfc86c2cb0a22156eef4cb5977476 #docking
)
FetchContent_Declare(
  stb 
  GIT_REPOSITORY https://github.com/nothings/stb
  GIT_TAG        beebb24b945efdea3b9bba23affb8eb3ba8982e7 #master
)
FetchContent_MakeAvailable(imgui stb)

if(PLATFORM_WINDOWS)
  set(imgui_platform_sources
    "${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp"
  )
endif()
if(PLATFORM_WASM)
  set(imgui_platform_sources
    "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_wgpu.cpp"
  )
endif()
set(imgui_sources
  "${imgui_SOURCE_DIR}/imgui.cpp"
  "${imgui_SOURCE_DIR}/imgui_demo.cpp"
  "${imgui_SOURCE_DIR}/imgui_draw.cpp"
  "${imgui_SOURCE_DIR}/imgui_tables.cpp"
  "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
  ${imgui_platform_sources}
  )
if(PLATFORM_WINDOWS)
  set(imgui_platform_libs
    "d3d11" "d3dcompiler" "Rpcrt4" "Advapi32" "Ole32" "Dwmapi" "gdi32" "Bcrypt"
  )
endif()
if(PLATFORM_WASM)
  set(imgui_platform_libs
    ""
  )
endif()
set(imgui_libs
  ${imgui_platform_libs}
  )
add_library(imgui STATIC ${imgui_sources})
target_compile_definitions(imgui PRIVATE UNICODE _UNICODE)
target_include_directories(imgui PRIVATE ${imgui_SOURCE_DIR})
target_sources(imgui PRIVATE ${imgui_sources})
target_link_libraries(imgui ${imgui_libs})

# -----------------------------------------------------------------------------------------------------

set(modules
  "src/helper.cpp"
  )
set(sources
  ${modules}
  "src/main-windows.cpp"
  "src/main-wasm.cpp"
  )

add_library(imgui_app_template STATIC ${sources})
target_compile_definitions(imgui_app_template PRIVATE UNICODE _UNICODE)
if(PLATFORM_WINDOWS)
  target_compile_definitions(imgui_app_template PRIVATE PLATFORM_WINDOWS)
endif()
if(PLATFORM_WASM)
  target_compile_definitions(imgui_app_template PRIVATE PLATFORM_WASM)
endif()
target_include_directories(imgui_app_template PRIVATE ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends ${stb_SOURCE_DIR})
target_sources(imgui_app_template PUBLIC FILE_SET CXX_MODULES FILES ${modules})
target_link_libraries(imgui_app_template imgui)
