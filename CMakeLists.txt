cmake_minimum_required(VERSION 3.12)

project(RaytracingAudio)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(HEADERS) )

set(HEADERS inc/Prerequisites.h
            inc/MathObjects.h
            inc/Image.h
            inc/Audio.h
            inc/tiny_obj_loader.h)

set(SOURCES src/main.cpp
            src/Audio.cpp
            src/Image.cpp)


add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)


