//
// Created by LONG on 2022/9/24.
//
#include <stdio.h>
#include <string.h>
#include "../include/disp_manager.h"


/*承上启下
 * 可以选择是使用LCD framebuffer 还是使用web或者其它来进行显示*/

static Display_device *display_dev = NULL;//链表头
static Display_device *display_default = NULL;
static Display_buffer display_buffer;
static int line_width;
static int pixel_width;

extern void framebuffer_register(void);//声明该函数，在其它文件寻找该函数

//在buton中间区域写text
int draw_text_central(char *name, Region *region, unsigned int color)
{
    int originX, originY;
    int i = 0;
    Font_bitmap fbm;

    Cartesian_region cartesianRegion;

    /*计算text的bbox框*/
    font_get_text_bbox(name, &cartesianRegion);

    /*计算第一个字符的原点*/
    originX = region->x + (region->width - cartesianRegion.width) / 2 - cartesianRegion.x;//画图很容易理解
    originY = region->y + (region->height - cartesianRegion.height) / 2 + cartesianRegion.y;

    /*绘制*/
    while (name[i]) {
        fbm.currOriginX = originX;
        fbm.currOriginY = originY;
        //获得点阵
        if (font_get_bitmap(name[i], &fbm)) {
            printf("get bitmap err!\n");
            return -1;
        }
        draw_font_bitmap(&fbm, color);
        //更新原点
        originX = fbm.nextOriginX;
        originY = fbm.nextOriginY;
        i++;
    }
    return 0;
}

//可用来画出button区域
void draw_region(Region *region, unsigned int color)
{
    int x = region->x;
    int y = region->y;
    int width = region->width;
    int height = region->height;
    int i, j;
    for (j = y; j < y + height; j++) {
        for (i = x; i < x + width; i++) {
            put_pixel(i, j, color);
        }
    }

}


void draw_font_bitmap(Font_bitmap *fbm, unsigned int color)
{
    int i, j, p, q;
    int x = fbm->region.x;
    int y = fbm->region.y;
    int x_max = x + fbm->region.width;
    int y_max = y + fbm->region.height;
    int width = fbm->region.width;
    unsigned char *buffer = fbm->buffer;

    //printf("x = %d, y = %d\n", x, y);

    for (j = y, q = 0; j < y_max; j++, q++) {
        for (i = x, p = 0; i < x_max; i++, p++) {
            if (i < 0 || j < 0 ||
                i >= display_buffer.xres || j >= display_buffer.yres)
                continue;//如果超过了屏幕范围，跳过当前循环，执行下一次

            //image[j][i] |= bitmap->buffer[q * bitmap->width + p];
            if (buffer[q * width + p])//buffer里为1就画点
                put_pixel(i, j, color);
        }
    }

}

int put_pixel(int x, int y, unsigned int color)
{
    unsigned char *pen_8 = (unsigned char *) (display_buffer.fb_base + y * line_width + x * pixel_width);
    unsigned short *pen_16;
    unsigned int *pen_32;

    unsigned int red, green, blue;

    pen_16 = (unsigned short *) pen_8;
    pen_32 = (unsigned int *) pen_8;

    switch (display_buffer.bpp) {
        case 8: {
            *pen_8 = color;
            break;
        }
        case 16: {
            /* 565 */
            red = (color >> 16) & 0xff;
            green = (color >> 8) & 0xff;
            blue = (color >> 0) & 0xff;
            color = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
            *pen_16 = color;
            break;
        }
        case 32: {
            *pen_32 = color;
            break;
        }
        default: {
            printf("can't surport %dbpp\n", display_buffer.bpp);
            return -1;

        }
    }
    return 0;
}

void register_display(Display_device *dev)
{
    dev->next = display_dev;//
    display_dev = dev;//注册上来
}

int select_default_display(char *name)
{
    Display_device *tmp = display_dev;
    while (tmp) {
        if (strcmp(name, tmp->name) == 0) {
            display_default = tmp;
            return 0;
        }
        tmp = tmp->next;
    }
    return -1;
}

int init_default_display(void)
{
    int ret;
    ret = display_default->device_init();
    if (ret) {
        printf("device_init err!\n");
        return -1;
    }
    ret = display_default->get_buffer(&display_buffer);
    if (ret) {
        printf("get_buffer err!\n");
        return -1;
    }
    line_width = display_buffer.xres * display_buffer.bpp / 8;
    pixel_width = display_buffer.bpp / 8;
    return 0;
}

int exit_default_display(void)
{
    display_default->device_exit();
    return 0;
}

Display_buffer *get_display_buffer(void)
{
    return &display_buffer;
}

int flush_display_region(Region *region, Display_buffer *buffer)
{
    return display_dev->flush_region(region, buffer);
}

void display_init()
{
    framebuffer_register();
}

