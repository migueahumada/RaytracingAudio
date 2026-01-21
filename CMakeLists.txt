#---------------------MAIN---------------------
cmake_minimum_required(VERSION 3.12)

project(RaytracingAudio)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} core/src/main.cpp)

# cl files
# set(CL_FILES ${CMAKE_CURRENT_SOURCE_DIR}/compute/compute.cl)
# target_sources(${PROJECT_NAME} PRIVATE ${CL_FILES})
# set_source_files_properties(${CL_FILES} PROPERTIES HEADER_FILE_ONLY ON)

# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC core/inc)

add_subdirectory(core)
add_subdirectory(tests)

target_link_libraries(${PROJECT_NAME} PUBLIC Core)
#Add OpenCL

# target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/OCL/include)
# target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libs/OCL/lib/x86_64/OpenCL.lib)






