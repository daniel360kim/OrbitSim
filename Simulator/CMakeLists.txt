cmake_minimum_required(VERSION 3.15)

project(OrbitOps)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
)


file(GLOB all_SRCS
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*.h
    ${PROJECT_SOURCE_DIR}/src/objects/*.cpp
    ${PROJECT_SOURCE_DIR}/src/objects/*.h
    ${PROJECT_SOURCE_DIR}/src/orbit/*.cpp
    ${PROJECT_SOURCE_DIR}/src/orbit/*.h
    ${PROJECT_SOURCE_DIR}/src/util/*.cpp
    ${PROJECT_SOURCE_DIR}/src/util/*.h
    ${PROJECT_SOURCE_DIR}/include/*.h
)

add_executable(${PROJECT_NAME} ${all_SRCS})

