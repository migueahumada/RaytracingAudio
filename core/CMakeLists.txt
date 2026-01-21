#-----------------------CORE---------------------------

set(SOURCES src/Audio.cpp
            src/Image.cpp
            src/DelayLine.cpp
            src/AudioBuffer.cpp
            src/Scene.cpp
            src/Viewport.cpp)
  #          src/ComputeAPI.cpp)

set(HEADERS inc/Audio.h
            inc/AudioBuffer.h
            inc/AudioHelpers.h
            inc/DelayLine.h
            inc/Image.h
            inc/Prerequisites.h
            inc/Scene.h
            inc/Viewport.h
            inc/tiny_obj_loader.h)

add_library(Core STATIC ${SOURCES} ${HEADERS})

target_include_directories(Core PUBLIC include inc)