cmake_minimum_required(VERSION 3.30.3)
project(3Dviewer)

include(FetchContent)
FetchContent_Declare(
  GlfwPackage
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.4
  )
  
FetchContent_MakeAvailable(GlfwPackage)
  
FILE(COPY res DESTINATION "${CMAKE_BINARY_DIR}")
include_directories(include)
add_executable(Main 
  src/Main.cpp 
  lib/error.cpp lib/glad.c lib/Shader.cpp lib/stb_image.cpp
  )

target_link_libraries(Main glfw Opengl32)
