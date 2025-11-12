cmake_minimum_required(VERSION 3.10)
project(WaterSimulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------
# Add GLAD
# -----------------------------
add_library(glad src/glad.c)
target_include_directories(glad PUBLIC include/)

# -----------------------------
# Fetch GLFW
# -----------------------------
include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)
FetchContent_MakeAvailable(glfw)

# -----------------------------
# Add ImGui
# -----------------------------
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)
target_link_libraries(imgui PUBLIC glfw glad)

# -----------------------------
# Find OpenGL
# -----------------------------
find_package(OpenGL REQUIRED)

if(TARGET OpenGL::GL)
    set(OPENGL_TARGET OpenGL::GL)
else()
    set(OPENGL_TARGET ${OPENGL_LIBRARIES})
endif()


# -----------------------------
# Add main executable
# -----------------------------
add_executable(WaterSimulation src/main.cpp)
target_include_directories(WaterSimulation PRIVATE include)

# -----------------------------
# Link dependencies
# -----------------------------
target_link_libraries(WaterSimulation
    PRIVATE
        imgui
        ${OPENGL_TARGET}
)
