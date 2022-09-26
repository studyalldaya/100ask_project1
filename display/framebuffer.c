//
// Created by LONG on 2022/9/24.
//
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include "../include/disp_manager.h"

/*使用LCD framebuffer， 最下层的程序
 * 如需使用其它设备显示，可再单独写一个C文件*/
static int fd_fb;
static struct fb_var_screeninfo var;    /* Current var */
static int screen_size;
static unsigned char *fb_base;
static unsigned int line_width;
static unsigned int pixel_width;

static int fb_device_init(void)
{
    fd_fb = open("/dev/fb0", O_RDWR);
    if (fd_fb < 0)
    {
        printf("can't open /dev/fb0\n");
        return -1;
    }
    if (ioctl(fd_fb, FBIOGET_VSCREENINFO, &var))
    {
        printf("can't get var\n");
        return -1;
    }

    line_width = var.xres * var.bits_per_pixel / 8;
    pixel_width = var.bits_per_pixel / 8;
    screen_size = var.xres * var.yres * var.bits_per_pixel / 8;
    fb_base = (unsigned char *) mmap(NULL, screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, 0);
    if (fb_base == (unsigned char *) -1)
    {
        printf("can't mmap\n");
        return -1;
    }
    return 0;

}

static int fb_device_exit(void)
{
    munmap(fb_base, screen_size);
    close(fd_fb);
    return 0;
}

/*可以返回LCD的framebuffer_base，xres,yres,bpp，上层APP可以直接操作LCD，不需要fb_flush_region
 * 也可以返回一个malloc的无关的内存，必须使用fb_flush_region。*/
static int fb_get_buffer(Display_buffer *disp_buffer)
{
    disp_buffer->xres = var.xres;
    disp_buffer->yres = var.yres;
    disp_buffer->bpp = var.bits_per_pixel;
    disp_buffer->fb_base = fb_base;
    return 0;
}

static int fb_flush_region(Region *region, Display_buffer *buffer)
{
    return 0;
}

static struct display_device framebuffer_dev = {
        .name = "fb",
        .device_init = fb_device_init,
        .device_exit = fb_device_exit,
        .get_buffer = fb_get_buffer,
        .flush_region = fb_flush_region,
};

void framebuffer_init(void)
{
    register_display(&framebuffer_dev);
}
