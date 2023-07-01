cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)

project(OrbitSim)

find_package(Vulkan REQUIRED)


include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/Dependencies/glfw/include
    ${PROJECT_SOURCE_DIR}/Dependencies/chernoimgui
    ${PROJECT_SOURCE_DIR}/Dependencies/dwalnut/Walnut/src

    # Vulkan
    $ENV{VULKAN_SDK}/Include

    # STB image
    ${PROJECT_SOURCE_DIR}/Dependencies/stb_image
)


link_directories(
    ${PROJECT_SOURCE_DIR}/Dependencies/glfw/lib-vc2022
    $ENV{VULKAN_SDK}/Lib
    $ENV{VULKAN_SDK}/Lib32
)

file(GLOB_RECURSE all_SRC
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*.h
    ${PROJECT_SOURCE_DIR}/include/*.h

    # GLM
    ${PROJECT_SOURCE_DIR}/Dependencies/glm/glm/*.inl

    # Walnut
    ${PROJECT_SOURCE_DIR}/Dependencies/dwalnut/Walnut/src/*.cpp
    ${PROJECT_SOURCE_DIR}/Dependencies/dwalnut/Walnut/src/*.h

    # Vulkan
    $ENV{VULKAN_SDK}/Include/vulkan/*.h

)

file(GLOB IMGUI_SRCS
    ${PROJECT_SOURCE_DIR}/Dependencies/chernoimgui/*.cpp
    ${PROJECT_SOURCE_DIR}/Dependencies/chernoimgui/*.h
    ${PROJECT_SOURCE_DIR}/Dependencies/chernoimgui/backends/imgui_impl_glfw.cpp
    ${PROJECT_SOURCE_DIR}/Dependencies/chernoimgui/backends/imgui_impl_glfw.h
    ${PROJECT_SOURCE_DIR}/Dependencies/chernoimgui/backends/imgui_impl_vulkan.cpp
    ${PROJECT_SOURCE_DIR}/Dependencies/chernoimgui/backends/imgui_impl_vulkan.h
    
)


add_executable(${PROJECT_NAME} ${all_SRCS} ${IMGUI_SRCS})

target_link_libraries(${PROJECT_NAME}
    glfw3
    opengl32
    vulkan-1



)


