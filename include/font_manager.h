//
// Created by LONG on 2022/9/28.
//

#ifndef INC_100ASK_PROJECT1_FONT_MANAGER_H
#define INC_100ASK_PROJECT1_FONT_MANAGER_H

#include "common.h"

typedef struct Font_bitmap {
    Region region;

    int currOriginX;//freetype 当前基点的x
    int currOriginY;
    int nextOriginX;
    int nextOriginY;

    unsigned char *buffer;
} Font_bitmap;

typedef struct Font_style {
    char *name;//用普通点阵还是使用freetype

    int (*font_init)(char *fname);
    int (*font_set_size)(int size);
    int (*font_get_bitmap)(unsigned int code, Font_bitmap *fbt);//code：编码值
    struct Font_style *next;
} Font_style;

void register_font_style(Font_style *fs);
void font_init(void);
int select_and_init_font(char *style, char *fname);
int font_set_size(int iFontSize);
int font_get_bitmap(unsigned int code, Font_bitmap *fbm);//code为字符的编码值，可以直接传入'A'

#endif //INC_100ASK_PROJECT1_FONT_MANAGER_H
