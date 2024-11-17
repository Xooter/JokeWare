cmake_minimum_required(VERSION 3.10)
set(PJ_NAME Joker)
project(${PJ_NAME} VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static -lpthread")
    target_link_libraries(${PJ_NAME} ws2_32)
elseif(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -pthread")
endif()

file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable(${PJ_NAME} main.cpp ${SRC_FILES})

target_compile_options(${PJ_NAME} PRIVATE -O2 -Wall)

target_include_directories(${PJ_NAME} PUBLIC src/${CMAKE_CURRENT_SOURCE_DIR} )
