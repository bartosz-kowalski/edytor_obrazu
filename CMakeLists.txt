cmake_minimum_required(VERSION 3.16)
project(edytor_obrazu CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall")

link_directories(${CMAKE_SOURCE_DIR}/raylib/lib)

include_directories(
    raylib/include
    zaleznosci
)

file(GLOB SRC_MAIN "edytor_obrazu.cpp")
file(GLOB SRC_DEPS "zaleznosci/*.cpp")

add_executable(edytor_obrazu
    ${SRC_MAIN}
    ${SRC_DEPS}
)

target_link_libraries(edytor_obrazu
     raylib
     opengl32
     gdi32
     winmm)

target_compile_definitions(edytor_obrazu PRIVATE
    TEXTURES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tekstury"
)


link_directories(raylib/lib)
