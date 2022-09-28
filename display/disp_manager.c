//
// Created by LONG on 2022/9/24.
//
#include <stdio.h>
#include <string.h>
#include "../include/disp_manager.h"

/*承上启下
 * 可以选择是使用LCD framebuffer 还是使用web或者其它来进行显示*/

static struct display_device *display_dev = NULL;//链表头
static struct display_device *display_default = NULL;
static Display_buffer display_buffer;
static int line_width;
static int pixel_width;

extern void framebuffer_register(void);//声明该函数，在其它文件寻找该函数

int put_pixel(int x, int y, unsigned int color)
{
    unsigned char *pen_8 = display_buffer.fb_base + y * line_width + x * pixel_width;
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
            break;
        }
    }
    return 0;
}

void register_display(struct display_device *dev)
{
    dev->next = display_dev;//
    display_dev = dev;//注册上来
}

int select_default_display(char *name)
{
    struct display_device *tmp = display_dev;
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
    display_dev->flush_region(region, buffer);
}

void display_init()
{
    framebuffer_register();
}

