cmake_minimum_required(VERSION 3.10)

project(killJava)

add_executable(killJava main.cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

install(TARGETS killJava DESTINATION $ENV{HOME}/bin)
