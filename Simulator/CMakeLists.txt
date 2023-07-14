cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)

project(OrbitSim)




include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include

    # Vulkan
    $ENV{VULKAN_SDK}/Include

    # GLFW
    ${PROJECT_SOURCE_DIR}/Dependencies/glfw/include

    #STB Image
    ${PROJECT_SOURCE_DIR}/Dependencies/stb_image

    #ImGui
    ${PROJECT_SOURCE_DIR}/Dependencies/imgui

    #glm
    ${PROJECT_SOURCE_DIR}/Dependencies/glmsm/glm

    #Walnut
    ${PROJECT_SOURCE_DIR}/Dependencies/myWalnut
    ${PROJECT_SOURCE_DIR}/Dependencies/myWalnut/Walnut

    # Glew
    ${PROJECT_SOURCE_DIR}/Dependencies/glew/include
)

link_directories(
    $ENV{VULKAN_SDK}/Lib
    ${PROJECT_SOURCE_DIR}/Dependencies/glfw/lib-vc2022
    ${PROJECT_SOURCE_DIR}/Dependencies/glew/lib/Release/x64
    
)



file(GLOB_RECURSE all_SRCS
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*.h
    ${PROJECT_SOURCE_DIR}/include/*.h
)

# glm
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

# ImGui
file(GLOB IMGUI_SRCS
    ${PROJECT_SOURCE_DIR}/Dependencies/imgui/*.cpp
    ${PROJECT_SOURCE_DIR}/Dependencies/imgui/*.h

)

# Walnut
file(GLOB WALNUT_SRCS
    ${PROJECT_SOURCE_DIR}/Dependencies/myWalnut/Walnut/*.cpp
    ${PROJECT_SOURCE_DIR}/Dependencies/myWalnut/Walnut/*.h
    ${PROJECT_SOURCE_DIR}/Dependencies/myWalnut/Walnut/ImGui/*.cpp
    ${PROJECT_SOURCE_DIR}/Dependencies/myWalnut/Walnut/ImGui/*.h
    ${PROJECT_SOURCE_DIR}/Dependencies/myWalnut/Walnut/Input/*.cpp
    ${PROJECT_SOURCE_DIR}/Dependencies/myWalnut/Walnut/Input/*.h
)

# Statically link GLEW
add_definitions(-DGLEW_STATIC)

add_executable(${PROJECT_NAME} ${all_SRCS} ${GLFW_SRCS} ${GLM_SRCS} ${IMGUI_SRCS} ${WALNUT_SRCS})

target_link_libraries(${PROJECT_NAME} 
    vulkan-1
    glfw3
    glew32s
    opengl32
)

