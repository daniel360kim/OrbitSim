cmake_minimum_required(VERSION 3.15)

project(OrbitOps)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/Dependencies/GLFW/include
)

link_directories(
    ${PROJECT_SOURCE_DIR}/Dependencies/GLFW/lib-vc2022
)


file(GLOB all_SRCS
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*.h
    ${PROJECT_SOURCE_DIR}/src/objects/*.cpp
    ${PROJECT_SOURCE_DIR}/src/objects/*.h
    ${PROJECT_SOURCE_DIR}/src/orbit/*.cpp
    ${PROJECT_SOURCE_DIR}/src/orbit/*.h
    ${PROJECT_SOURCE_DIR}/src/visualization/*.cpp
    ${PROJECT_SOURCE_DIR}/src/visualization/*.h
    ${PROJECT_SOURCE_DIR}/src/visualization/Events/*.cpp
    ${PROJECT_SOURCE_DIR}/src/visualization/Events/*.h
)

add_executable(${PROJECT_NAME} ${all_SRCS})

target_link_libraries(${PROJECT_NAME}
    glfw3
    opengl32
)
