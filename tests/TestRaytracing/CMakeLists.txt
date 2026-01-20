cmake_minimum_required(VERSION 3.12)

project(TestRaytracing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES src/RaytracingTests.cpp)

set(HEADERS 
	../../inc/MathObjects.h
	../../inc/Prerequisites.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC ../../inc)

