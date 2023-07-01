cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)

project(OrbitSim)

find_package(Vulkan REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/Dependencies/glfw/include
    ${PROJECT_SOURCE_DIR}/Dependencies/dimgui
)

link_directories(
    ${PROJECT_SOURCE_DIR}/Dependencies/glfw/lib-vc2022
)

file(GLOB_RECURSE all_SRCS
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*.h
    ${PROJECT_SOURCE_DIR}/include/*.h

    # GLM
    ${PROJECT_SOURCE_DIR}/Dependencies/glm/glm/*.inl

)

file(GLOB IMGUI_SRCS
    ${PROJECT_SOURCE_DIR}/Dependencies/dimgui/*.cpp
    ${PROJECT_SOURCE_DIR}/Dependencies/dimgui/*.h
    ${PROJECT_SOURCE_DIR}/Dependencies/dimgui/backends/imgui_impl_glfw.cpp
    ${PROJECT_SOURCE_DIR}/Dependencies/dimgui/backends/imgui_impl_glfw.h
    ${PROJECT_SOURCE_DIR}/Dependencies/dimgui/backends/imgui_impl_vulkan.cpp
    ${PROJECT_SOURCE_DIR}/Dependencies/dimgui/backends/imgui_impl_vulkan.h
)


add_executable(${PROJECT_NAME} ${all_SRCS} ${IMGUI_SRCS})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    glfw3
    opengl32
    ${Vulkan_LIBRARIES}
)


