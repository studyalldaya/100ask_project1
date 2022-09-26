//
// Created by LONG on 2022/9/24.
//

#ifndef INC_100ASK_PROJECT1_DISP_MANAGER_H
#define INC_100ASK_PROJECT1_DISP_MANAGER_H

typedef struct display_buffer {
    int xres;
    int yres;
    int bpp;
    char *fb_base;
} Display_buffer;

typedef struct region {
    int x;//左上角x
    int y;//左上角y
    int height;
    int width;
} Region;

struct display_device {
    char *name;

    //函数指针用作多态
    int (*device_init)(void);//open
    int (*device_exit)(void);//close
    int (*get_buffer)(Display_buffer *disp_buffer);

    int (*flush_region)(Region *region, Display_buffer *buffer);

    struct display_device *next;//支持多个输出  lcd or web
};

void register_display(struct display_device *dev);


// api
void display_init(void);

int select_default_display(char *name);

int init_default_display(void);

int put_pixel(int x, int y, unsigned int color);

Display_buffer *get_display_buffer(void);

int flush_display_region(Region *region, Display_buffer *buffer);

int exit_default_display(void);

#endif //INC_100ASK_PROJECT1_DISP_MANAGER_H
