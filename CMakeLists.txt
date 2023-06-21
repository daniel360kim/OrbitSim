cmake_minimum_required(VERSION 3.15)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
)

file(GLOB all_SRCS
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*.h
    ${PROJECT_SOURCE_DIR}/src/objects/*.cpp
    ${PROJECT_SOURCE_DIR}/src/objects/*.h

)

add_executable(${PROJECT_NAME} ${all_SRCS})