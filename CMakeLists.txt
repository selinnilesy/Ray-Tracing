# CMakeList.txt : CMake project for ceng477hw1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("ceng477hw1")

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O3 -Wall ")


# Add source to this project's executable.
find_package (Threads)
add_executable (raytracer "parser.cpp" "parser.h" "ppm.cpp" "ppm.h" "raytracer.cpp" "tinyxml2.cpp" "tinyxml2.h" "Algebra.h" "Algebra.cpp" "Ray.h" "Ray.cpp" "CameraHandler.h" "CameraHandler.cpp" "SceneHandler.h" "SceneHandler.cpp")
target_link_libraries (raytracer ${CMAKE_THREAD_LIBS_INIT})

# TODO: Add tests and install targets if needed.
