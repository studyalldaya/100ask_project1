
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./include/disp_manager.h"
#include "./include/font_manager.h"
#include "./include/UI.h"
#include "./include/input_manager.h"
#include "./include/page_manager.h"
#include "./include/config.h"

#if 1

//总
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
        printf("Usage: %s <font_file> \n", argv[0]);
        return -1;
    }

    /*初始化显示系统*/
    display_init();
    select_default_display("fb");
    init_default_display();
    //ptBuffer = get_display_buffer();

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

    /*读取配置文件*/
    // parse_config_file(argv[2]);
    /*运行业务系统主页面*/
    get_page("main")->run(NULL);


    return 0;
}

#endif

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

#if 0

//text text
int main(int argc, char **argv)
{
    Display_buffer *ptBuffer;
    int error;

    Font_bitmap tFontBitMap;
    char *str = "www.100ask.net";
    int i = 0;
    int lcd_x;
    int lcd_y;
    int font_size;


    if (argc != 5) {
        printf("Usage: %s <font_file> <lcd_x> <lcd_y> <font_size>\n", argv[0]);
        return -1;
    }

    lcd_x = strtol(argv[2], NULL, 0);
    lcd_y = strtol(argv[3], NULL, 0);

    font_size = strtol(argv[4], NULL, 0);


    display_init();

    select_default_display("fb");

    init_default_display();

    ptBuffer = get_display_buffer();


    font_init();

    error = select_and_init_font("freetype", argv[1]);
    if (error) {
        printf("SelectAndInitFont err\n");
        return -1;
    }

    font_set_size(font_size);

    while (str[i]) {
        /* get bitmap */
        tFontBitMap.currOriginX = lcd_x;
        tFontBitMap.currOriginY = lcd_y;
        error = font_get_bitmap(str[i], &tFontBitMap);
        if (error) {
            printf("SelectAndInitFont err\n");
            return -1;
        }

        /* draw on buffer */
        draw_font_bitmap(&tFontBitMap, 0xff0000);

        /* flush to lcd/web */
        flush_display_region(&tFontBitMap.region, ptBuffer);


        lcd_x = tFontBitMap.nextOriginX;
        lcd_y = tFontBitMap.nextOriginY;
        i++;
    }

    return 0;
}

#endif