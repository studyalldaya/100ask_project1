cmake_minimum_required(VERSION 3.10)
project(100ask_project1 C)

set(CMAKE_C_STANDARD 99)

add_executable(100ask_project1 main.c include/disp_manager.h display/framebuffer.c display/disp_manager.c)
