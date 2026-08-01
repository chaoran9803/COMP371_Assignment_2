cmake_minimum_required(VERSION 3.6)
project(OpenGLIntro)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)

# Prefer pkg-config for Linux libraries
pkg_check_modules(GLFW3 REQUIRED glfw3)
pkg_check_modules(GLEW REQUIRED glew)
pkg_check_modules(GLM REQUIRED glm)

include_directories(${GLFW3_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})
link_directories(${GLFW3_LIBRARY_DIRS} ${GLEW_LIBRARY_DIRS} ${GLM_LIBRARY_DIRS})

add_executable(OpenGLIntro OpenGLIntro.cpp)

target_compile_definitions(OpenGLIntro PRIVATE ${GLFW3_CFLAGS_OTHER} ${GLEW_CFLAGS_OTHER})

target_link_libraries(OpenGLIntro PRIVATE ${OPENGL_LIBRARIES} ${GLFW3_LIBRARIES} ${GLEW_LIBRARIES} ${GLM_LIBRARIES} dl)

# Ensure rpath on Linux so shared libs in system paths are found
set_target_properties(OpenGLIntro PROPERTIES BUILD_RPATH "${CMAKE_INSTALL_RPATH}")
