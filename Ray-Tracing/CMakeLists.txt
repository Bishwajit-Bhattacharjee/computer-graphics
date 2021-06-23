cmake_minimum_required(VERSION 3.17)
project(Ray_Tracing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(OpenGlLinkers -lglut -lGLU -lGL) # setting all the Glut libraries as one variable.

add_executable(Ray_Tracing 1605003_main.cpp)

target_link_libraries(Ray_Tracing ${OpenGlLinkers}) # linking opengl libraries to the project
