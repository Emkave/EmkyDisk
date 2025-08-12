cmake_minimum_required(VERSION 3.31)
project(EmkyDisk)

set(CMAKE_CXX_STANDARD 23)

add_executable(EmkyDisk main.cpp
        disk.h
        settings.h
        segment.h
        segment.cpp
        disk.cpp
)


set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR D:/SFML-3.0.0/lib/cmake/SFML)
set(Ogg_DIR D:/SFML-3.0.0/lib/cmake/Ogg)
find_package(SFML 3 REQUIRED COMPONENTS System Window Graphics Audio Network)
include_directories(D:/SFML-3.0.0/include/SFML)

set(GLFW_LIB_DIR ${CMAKE_SOURCE_DIR}/lib-vc2019)
link_directories(${GLFW_LIB_DIR})

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/glad)
include_directories(${CMAKE_SOURCE_DIR}/include/KHR)
include_directories(${CMAKE_SOURCE_DIR})

#set(VLD_DIR "D:/Visual Leak Detector")
#include_directories(${VLD_DIR}/include)
#link_directories(${VLD_DIR}/lib/Win64)
#add_definitions(-DVLDBUILD)

target_link_libraries(EmkyDisk PRIVATE glfw3 opengl32 SFML::System SFML::Window SFML::Graphics SFML::Audio)
target_include_directories(EmkyDisk PRIVATE ${CMAKE_SOURCE_DIR}/glad ${CMAKE_SOURCE_DIR}/include ${GLFW_LIB_DIR})