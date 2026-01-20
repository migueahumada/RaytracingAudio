cmake_minimum_required(VERSION 3.12)

project(RaytracingTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES ../src/Audio.cpp
            src/main.cpp)

set(HEADERS ../inc/Audio.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC ../inc )


