cmake_minimum_required(VERSION 3.8)
project(learn_opengl)

set(CMAKE_CXX_STANDARD 11)

# config glfw
option(GL_SILENCE_DEPRECATION OFF)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
find_package(OpenGL REQUIRED)
add_subdirectory(lib/glfw)

# config glad
#set(GLAD_SRC lib/glad-c/src/glad.c)
#if (NOT WIN32)
#    LIST(APPEND LIBS dl)
#endif ()
#include_directories(lib/glad-c/include)

# config glm
#include_directories(lib/glm)

#set(SOURCE_FILES main.cpp ${GLAD_SRC})
set(SOURCE_FILES main.cpp)
add_executable(learn_opengl ${SOURCE_FILES})
target_link_libraries(learn_opengl glfw)
target_link_libraries(learn_opengl OpenGL::GL)
target_link_libraries(learn_opengl ${LIBS})