cmake_minimum_required(VERSION 3.10)
set(PJ_NAME Joker)
project(${PJ_NAME} VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(${PJ_NAME} main.cpp ${SRC_FILES})
target_compile_options(${PJ_NAME} PRIVATE -O2 -Wall)
target_include_directories(${PJ_NAME} PUBLIC src/${CMAKE_CURRENT_SOURCE_DIR} )

target_link_libraries(${PJ_NAME}  ws2_32)
