#---------------------TESTING---------------------

cmake_minimum_required(VERSION 3.12)

project(UnitTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} src/Tests.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC Core)

target_include_directories(${PROJECT_NAME} PUBLIC include ../core/inc)


