cmake_minimum_required(VERSION 3.22)
project(socketClient C)

set(CMAKE_C_STANDARD 99)


find_path(TheHeaderFile sock_utils.h PATHS /home/mvoii/Desktop/chatinCnc/socketUtils/)

find_library(TheLibrary NAMES socketUtils PATHS /home/mvoii/Desktop/chatinCnc/socketUtils/)

add_executable(socketClient main.c)

include_directories(${TheHeaderFile})
target_link_libraries(${PROJECT_NAME} ${TheLibrary})
