cmake_minimum_required(VERSION 3.12)

project(RaytracingAudio)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS inc/Prerequisites.h
            inc/MathObjects.h
            inc/Image.h
            inc/Audio.h
            inc/RaytracingHelpers.h
            inc/AudioHelpers.h
            inc/DelayLine.h
            inc/AudioBuffer.h
            inc/tiny_obj_loader.h)
#            inc/ComputeAPI.h)

set(SOURCES src/main.cpp
            src/Audio.cpp
            src/Image.cpp
            src/DelayLine.cpp
            src/AudioBuffer.cpp)
  #          src/ComputeAPI.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# cl files
# set(CL_FILES ${CMAKE_CURRENT_SOURCE_DIR}/compute/compute.cl)
# target_sources(${PROJECT_NAME} PRIVATE ${CL_FILES})
# set_source_files_properties(${CL_FILES} PROPERTIES HEADER_FILE_ONLY ON)




target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

add_subdirectory(tests/TestRaytracing)
add_subdirectory(tests/TestAudio)
#Add OpenCL

# target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/OCL/include)
# target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libs/OCL/lib/x86_64/OpenCL.lib)






