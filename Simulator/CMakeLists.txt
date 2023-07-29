# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.15)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)

# Project name: OrbitSim
project(OrbitSim)

# Set the build type ("Debug" or "Dist")
set(BUILD_SETTING "Dist")
set(CMAKE_INSTALL_CONFIG_NAME Debug)

# Ensure that the project is only supported on Windows and MSVC
if (NOT WIN32)
    message(FATAL_ERROR "This project is only supported on Windows")
elseif (NOT MSVC)
    message(FATAL_ERROR "This project is only supported on MSVC")
endif()

# Include directories for various dependencies
include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src/core
    $ENV{VULKAN_SDK}/Include
    ${PROJECT_SOURCE_DIR}/Dependencies/glfw/include
    ${PROJECT_SOURCE_DIR}/Dependencies/stb_image
    ${PROJECT_SOURCE_DIR}/Dependencies/imgui
    ${PROJECT_SOURCE_DIR}/Dependencies/glmsm/glm
    ${PROJECT_SOURCE_DIR}/Dependencies/mainWalnut
    ${PROJECT_SOURCE_DIR}/Dependencies/mainWalnut/Walnut
    ${PROJECT_SOURCE_DIR}/Dependencies/glew/include
)

# Link directories for various dependencies
link_directories(
    $ENV{VULKAN_SDK}/Lib
    ${PROJECT_SOURCE_DIR}/Dependencies/glfw/lib-vc2022
    ${PROJECT_SOURCE_DIR}/Dependencies/glew/lib/Release/x64
)

# Gather all source files in the project
file(GLOB_RECURSE all_SRCS
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*.h
    ${PROJECT_SOURCE_DIR}/include/*.h
)

# Gather source files for glm, ImGui, and Walnut
file(GLOB GLM_SRCS
    ${PROJECT_SOURCE_DIR}/Dependencies/glmsm/glm/*.cpp
    ${PROJECT_SOURCE_DIR}/Dependencies/glmsm/glm/*.h
    ${PROJECT_SOURCE_DIR}/Dependencies/glmsm/glm/*.inl
    ${PROJECT_SOURCE_DIR}/Dependencies/glmsm/glm/gtc/*.cpp
    ${PROJECT_SOURCE_DIR}/Dependencies/glmsm/glm/gtc/*.h
    ${PROJECT_SOURCE_DIR}/Dependencies/glmsm/glm/gtc/*.inl
    ${PROJECT_SOURCE_DIR}/Dependencies/glmsm/glm/ext/*.cpp
    ${PROJECT_SOURCE_DIR}/Dependencies/glmsm/glm/ext/*.h
    ${PROJECT_SOURCE_DIR}/Dependencies/glmsm/glm/ext/*.inl
)

file(GLOB IMGUI_SRCS
    ${PROJECT_SOURCE_DIR}/Dependencies/imgui/*.cpp
    ${PROJECT_SOURCE_DIR}/Dependencies/imgui/*.h
)

file(GLOB WALNUT_SRCS
    ${PROJECT_SOURCE_DIR}/Dependencies/mainWalnut/Walnut/*.cpp
    ${PROJECT_SOURCE_DIR}/Dependencies/mainWalnut/Walnut/*.h
    ${PROJECT_SOURCE_DIR}/Dependencies/mainWalnut/Walnut/ImGui/*.cpp
    ${PROJECT_SOURCE_DIR}/Dependencies/mainWalnut/Walnut/ImGui/*.h
    ${PROJECT_SOURCE_DIR}/Dependencies/mainWalnut/Walnut/Input/*.cpp
    ${PROJECT_SOURCE_DIR}/Dependencies/mainWalnut/Walnut/Input/*.h
)

# Set icon rc file for the project
file (GLOB ICON_SRCS "${PROJECT_SOURCE_DIR}/assets/Icons/*.rc")

# Statically link GLEW and set Walnut platform to Windows
add_definitions(-DGLEW_STATIC)
add_definitions(-DWL_PLATFORM_WINDOWS)

# Conditionally set the target based on the build setting
if (BUILD_SETTING STREQUAL "Debug")
    add_executable(${PROJECT_NAME} ${all_SRCS} ${GLM_SRCS} ${IMGUI_SRCS} ${WALNUT_SRCS})
    message("Debug Build")
elseif (BUILD_SETTING STREQUAL "Dist")
    add_definitions(-DWL_DIST)
    add_executable(${PROJECT_NAME} WIN32 ${all_SRCS} ${GLM_SRCS} ${IMGUI_SRCS} ${WALNUT_SRCS} ${ICON_SRCS})
    message("Dist Build")
endif()

# Link required libraries
target_link_libraries(${PROJECT_NAME}
    vulkan-1
    glfw3
    glew32s
    opengl32
)

# CPack configuration for packaging the project\
set(ASSETS_SOURCE_DIR "${PROJECT_SOURCE_DIR}/assets")
set(ASSETS_DEST_DIR "${CMAKE_BINARY_DIR}/Debug/assets")
set(OUT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/out")
set(OUT_DEST_DIR "${CMAKE_BINARY_DIR}/Debug/out")
set(IMGUI_INI_SOURCE_FILE "${PROJECT_BINARY_DIR}/Debug/imgui.ini")


# Copy assets to the build directory
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${OUT_SOURCE_DIR} ${OUT_DEST_DIR}
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${ASSETS_SOURCE_DIR} ${ASSETS_DEST_DIR}
)

# Install commands for distributing the project
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY ${ASSETS_SOURCE_DIR} DESTINATION bin)
install(DIRECTORY ${OUT_SOURCE_DIR} DESTINATION bin)
install(FILES ${IMGUI_INI_SOURCE_FILE} DESTINATION bin)

# Include required system libraries in the CPack package
include(InstallRequiredSystemLibraries)

# Configure CPack settings for generating a ZIP package
set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_VENDOR "Daniel Kim")
set(CPACK_OUTPUT_FILE_PREFIX "../bin")

# Include CPack for generating the package
set(CMAKE_INSTALL_CONFIG_NAME "Debug")
include(CPack)


