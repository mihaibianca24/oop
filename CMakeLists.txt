cmake_minimum_required(VERSION 4.1)
project(oop)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_OSX_SYSROOT /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk)

add_executable(oop
        projects/project1/main.cpp)