cmake_minimum_required(VERSION 3.15)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
)

file(GLOB all_SRCS
    ${PROJECT_SOURCE_DIR}/src/*.cpp

)

add_executable(${PROJECT_NAME} ${all_SRCS})