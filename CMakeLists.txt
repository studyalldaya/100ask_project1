cmake_minimum_required(VERSION 3.10)
project(100ask_project1 C)

set(CMAKE_C_STANDARD 99)
#添加tslib库
set(INC_DIR ~/100ask_imx6ull-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/include)
set(LINK_DIR ~/100ask_imx6ull-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/lib)
include_directories(
        ${INC_DIR}
        ${INC_DIR}/freetype2
)

link_directories(${LINK_DIR})

add_executable(100ask_project1 main.c include/disp_manager.h display/framebuffer.c display/disp_manager.c include/input_manager.h input/touchscreen.c input/net_input.c input/input_manager.c include/font_manager.h font/font_freetype.c font/font_manager.c include/common.h include/UI.h UI/button.c include/page_manager.h page/page_manager.c page/main_page.c include/config.h config/config.c)

target_link_libraries(100ask_project1 ts pthread freetype)