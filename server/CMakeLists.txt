cmake_minimum_required(VERSION 3.10)
set(PJ_NAME Batman)
project(${PJ_NAME} VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CURL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/dependencies/curl/include")
set(CURL_LIB_DIR "${CMAKE_SOURCE_DIR}/dependencies/curl/lib")

find_library(CURL_LIBRARY NAMES curl PATHS ${CURL_LIB_DIR})

if(NOT CURL_LIBRARY)
    message(FATAL_ERROR "CURL library not found")
endif()

file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable(${PJ_NAME} main.cpp ${SRC_FILES})

target_compile_options(${PJ_NAME} PRIVATE -O2 -Wall)

target_include_directories(${PJ_NAME} PUBLIC ${CURL_INCLUDE_DIR})

target_link_libraries(${PJ_NAME} ${CURL_LIBRARY} ws2_32)
