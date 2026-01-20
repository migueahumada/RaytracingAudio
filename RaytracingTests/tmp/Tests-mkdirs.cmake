# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "D:/Coding/C++/RaytracingAudio/RaytracingAudio/tests/src/Tests")
  file(MAKE_DIRECTORY "D:/Coding/C++/RaytracingAudio/RaytracingAudio/tests/src/Tests")
endif()
file(MAKE_DIRECTORY
  "D:/Coding/C++/RaytracingAudio/RaytracingAudio/tests/src/Tests-build"
  "D:/Coding/C++/RaytracingAudio/RaytracingAudio/tests"
  "D:/Coding/C++/RaytracingAudio/RaytracingAudio/tests/tmp"
  "D:/Coding/C++/RaytracingAudio/RaytracingAudio/tests/src/Tests-stamp"
  "D:/Coding/C++/RaytracingAudio/RaytracingAudio/tests/src"
  "D:/Coding/C++/RaytracingAudio/RaytracingAudio/tests/src/Tests-stamp"
)

set(configSubDirs Debug;Release;MinSizeRel;RelWithDebInfo)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Coding/C++/RaytracingAudio/RaytracingAudio/tests/src/Tests-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Coding/C++/RaytracingAudio/RaytracingAudio/tests/src/Tests-stamp${cfgdir}") # cfgdir has leading slash
endif()
