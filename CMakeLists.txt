cmake_minimum_required(VERSION 3.8)
project(draw_triangle)

set(CMAKE_CXX_STANDARD 11)

# config glfw
option(GL_SILENCE_DEPRECATION OFF)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
find_package(OpenGL REQUIRED)
add_subdirectory(lib/glfw)

# config glad
set(GLAD_SRC lib/glad-c/src/glad.c)
if (NOT WIN32)
    LIST(APPEND LIBS dl)
endif ()
include_directories(lib/glad-c/include)

# config glm
include_directories(lib/glm)

# stb_image
include_directories(lib)

# util
include_directories(utils)

set(SOURCE_FILES Light/main.cpp ${GLAD_SRC} utils/image_util.cpp utils/OurShader.h head/my_structs.h)
add_executable(draw_triangle ${SOURCE_FILES})
target_link_libraries(draw_triangle glfw)
target_link_libraries(draw_triangle OpenGL::GL)
target_link_libraries(draw_triangle ${LIBS})