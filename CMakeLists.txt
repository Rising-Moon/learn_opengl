cmake_minimum_required(VERSION 3.8)
project(opengl)

set(CMAKE_CXX_STANDARD 11)

# config glfw
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(lib/glfw)
include_directories(lib/glfw/include)
LIST(APPEND LIBS lib/glfw ${GLFW_LIBRARIES})

# config glad
set(GLAD_SRC lib/glad-c/src/glad.c)
if (NOT WIN32)
    LIST(APPEND LIBS dl)
endif ()
include_directories(lib/glad-c/include)

# config glm
include_directories(lib/glm)

set(SOURCE_FILES main.cpp ${GLAD_SRC})
add_executable(opengl ${SOURCE_FILES})
target_link_libraries(opengl ${LIBS})