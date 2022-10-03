
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./include/disp_manager.h"
#include "./include/font_manager.h"
#include "./include/UI.h"
#include "./include/input_manager.h"
#include "./include/page_manager.h"


int main(int argc, char **argv)
{
    Display_buffer *ptBuffer;
    int error;

    Font_bitmap tFontBitMap;
    char *str = "www.100ask.net";
    int i = 0;
    int lcd_x;
    int lcd_y;

    if (argc != 2) {
        printf("Usage: %s <font_file>\n", argv[0]);
        return -1;
    }

    /*初始化显示系统*/
    display_init();
    select_default_display("fb");
    init_default_display();
    ptBuffer = get_display_buffer();

    /*初始化输入系统*/
    input_init();
    input_device_init();

    /*初始化文字系统*/
    font_init();
    error = select_and_init_font("freetype", argv[1]);
    if (error) {
        printf("SelectAndInitFont err\n");
        return -1;
    }

    /*初始化页面系统*/
    page_init();

    /*运行业务系统主页面*/
    get_page("main")->run(NULL);


    return 0;
}

#if 0
/* ---- button UI test ---- */
int main(int argc, char **argv)
{
    Display_buffer *ptBuffer;
    int error;
    Button tButton;
    Region tRegion;

    if (argc != 2) {
        printf("Usage: %s <font_file_path>\n", argv[0]);
        return -1;
    }

    display_init();
    select_default_display("fb");

    init_default_display();

    ptBuffer = get_display_buffer();
    font_init();
    //FontsRegister();
    if (select_and_init_font("freetype", argv[1])) {
        printf("SelectAndInitFont err\n");
        return -1;
    }

    tRegion.x = 200;
    tRegion.y = 200;
    tRegion.width = 300;
    tRegion.height = 100;

    init_button(&tButton, "test", &tRegion, NULL, NULL);
    tButton.on_draw(&tButton, ptBuffer);
    while (1) {
        tButton.on_clicked(&tButton, ptBuffer, NULL);
        sleep(2);
    }

    return 0;
}
#endif
