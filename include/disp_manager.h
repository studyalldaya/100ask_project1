//
// Created by LONG on 2022/9/24.
//

#ifndef INC_100ASK_PROJECT1_DISP_MANAGER_H
#define INC_100ASK_PROJECT1_DISP_MANAGER_H

#include "common.h"
#include "../include/font_manager.h"

typedef struct Display_buffer {
    int xres;
    int yres;
    int bpp;
    char *fb_base;
} Display_buffer;


typedef struct Display_device {
    char *name;

    //函数指针用作多态
    int (*device_init)(void);//open
    int (*device_exit)(void);//close
    int (*get_buffer)(Display_buffer *disp_buffer);

    int (*flush_region)(Region *region, Display_buffer *buffer);

    struct Display_device *next;//支持多个输出  lcd or web
} Display_device;

void register_display(Display_device *dev);


// api
void display_init(void);

int select_default_display(char *name);

int init_default_display(void);

int put_pixel(int x, int y, unsigned int color);

void draw_font_bitmap(Font_bitmap *fbm, unsigned int color);

void draw_region(Region *region, unsigned int color);

int draw_text_central(char *name, Region *region, unsigned int color);

Display_buffer *get_display_buffer(void);

int flush_display_region(Region *region, Display_buffer *buffer);

int exit_default_display(void);

#endif //INC_100ASK_PROJECT1_DISP_MANAGER_H
