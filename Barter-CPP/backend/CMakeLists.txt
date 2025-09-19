cmake_minimum_required(VERSION 3.20)
project(barterloops_engine VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(barterloops_engine src/main.cpp)
target_include_directories(barterloops_engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
