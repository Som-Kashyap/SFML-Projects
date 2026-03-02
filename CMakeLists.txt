cmake_minimum_required(VERSION 3.10)
project(RaylibProject)

set(CMAKE_CXX_STANDARD 17)

include_directories(C:/raylib/include)
link_directories(C:/raylib/lib)

add_executable(main main.cpp)

target_link_libraries(main raylib opengl32 gdi32 winmm)