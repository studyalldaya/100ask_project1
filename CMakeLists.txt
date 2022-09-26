cmake_minimum_required(VERSION 3.10)
project(100ask_project1 C)

set(CMAKE_C_STANDARD 99)
#添加tslib库
set(INC_DIR ~/100ask_imx6ull-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/include)
set(LINK_DIR ~/100ask_imx6ull-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/lib)
include_directories(${INC_DIR/tslib.h})
link_directories(${LINK_DIR})

add_executable(100ask_project1 main.c include/disp_manager.h display/framebuffer.c display/disp_manager.c include/input_manager.h input/touchscreen.c)

target_link_libraries(100ask_project1 ts)