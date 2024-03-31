cmake_minimum_required(VERSION 3.28)
project(imgui_app_template LANGUAGES C CXX)

include(FetchContent)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

# -----------------------------------------------------------------------------------------------------

FetchContent_Declare(
  imgui 
  GIT_REPOSITORY https://github.com/ocornut/imgui
  GIT_TAG        docking
)
FetchContent_Declare(
  stb 
  GIT_REPOSITORY https://github.com/nothings/stb
  GIT_TAG        master
)
FetchContent_MakeAvailable(imgui stb)

if(PLATFORM_WINDOWS)
  set(imgui_platform_sources
    "${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp"
  )
endif()
if(PLATFORM_SDL2GL3)
  set(imgui_platform_sources
    "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
  )
endif()
if(PLATFORM_GLFWGL3)
  set(imgui_platform_sources
    "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
  )
endif()
if(PLATFORM_WASM)
  set(imgui_platform_sources
    "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
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
    d3d11 d3dcompiler Rpcrt4 Advapi32 Ole32 Dwmapi gdi32
  )
endif()
if(PLATFORM_SDL2GL3)
  set(imgui_platform_libs
    ""
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

#add_custom_target(imgui_modules
#  COMMAND clang++ -std=c++23 -xc++-system-header --precompile ${imgui_SOURCE_DIR}/imgui.h -o imgui.pcm
#  VERBATIM
#)

# -----------------------------------------------------------------------------------------------------

set(modules
  "src/helper.cpp"
  )
set(sources
  ${modules}
  "src/main-windows.cpp"
  "src/main-wasm.cpp"
  "src/main-sdl2gl3.cpp"
  "src/main-glfwgl3.cpp"
  )

add_library(imgui_app_template STATIC ${sources})
target_compile_definitions(imgui_app_template PRIVATE UNICODE _UNICODE)
if(PLATFORM_WINDOWS)
  target_compile_definitions(imgui_app_template PRIVATE PLATFORM_WINDOWS)
endif()
if(PLATFORM_WASM)
  target_compile_definitions(imgui_app_template PRIVATE PLATFORM_WASM)
  target_link_options(imgui_app_template PRIVATE -s DISABLE_EXCEPTION_CATCHING=1)
  target_compile_options(imgui_app_template PRIVATE -s DISABLE_EXCEPTION_CATCHING=1)
  target_link_options(imgui_app_template PRIVATE -s USE_GLFW=3 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s NO_EXIT_RUNTIME=0 -s ASSERTIONS=1)
endif()
if(PLATFORM_SDL2GL3)
  find_package(SDL2 REQUIRED)
  find_package(OpenGL REQUIRED)
  include_directories(imgui_app_template ${SDL2_INCLUDE_DIRS})
  target_link_libraries(imgui_app_template SDL2::SDL2-static)
  target_link_libraries(imgui_app_template OpenGL::GL)
endif()
if(PLATFORM_GLFWGL3)
  target_compile_definitions(imgui_app_template PRIVATE PLATFORM_GLFWGL3)
  find_package(glfw3 3.3 REQUIRED)
  find_package(OpenGL REQUIRED)
  target_link_libraries(imgui_app_template glfw3)
  target_link_libraries(imgui_app_template OpenGL::GL)
endif()
target_include_directories(imgui_app_template PRIVATE ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends ${stb_SOURCE_DIR})
#target_sources(imgui_app_template PUBLIC FILE_SET CXX_MODULES FILES ${modules})
target_link_libraries(imgui_app_template imgui)
#target_compile_options(imgui_app_template PRIVATE "-fmodule-file=${imgui_app_template_BINARY_DIR}/imgui.pcm")
#add_dependencies(imgui_app_template imgui_modules)
