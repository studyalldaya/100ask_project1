//
// Created by LONG on 2022/9/29.
//
#include <ft2build.h>
#include <stdio.h>
#include "../include/font_manager.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H

static FT_Face face;
static int defaultFontSize = 12;

static int freetype_font_init(char *file_path)
{
    int ret;
    FT_Library library;
    /* 显示矢量字体 */
    ret = FT_Init_FreeType(&library);               /* initialize library */
    if (ret) {
        printf("FT_Init_FreeType err\n");
        return -1;
    }
    /* error handling omitted */
    ret = FT_New_Face(library, file_path, 0, &face); /* create face object */
    if (ret) {
        printf("FT_New_Face err\n");
        return -1;
    }
    //设置成12*12像素，  0表示和另一个值相等！
    FT_Set_Pixel_Sizes(face, defaultFontSize, 0);
    return 0;
}

static int freetype_set_size(int size)
{
    FT_Set_Pixel_Sizes(face, size, 0);
    return 0;
}

static int freetype_get_bitmap(unsigned int code, Font_bitmap *fbt)
{
    int error;
    FT_Vector pen;
    FT_Glyph glyph;
    FT_GlyphSlot slot;
    /*得到开始位置*/
    pen.x = fbt->currOriginX * 64; /* 单位: 1/64像素 */
    pen.y = fbt->currOriginY * 64; /* 单位: 1/64像素 */
    /* 设置旋转以及位置 */
    FT_Set_Transform(face, 0, &pen);//0表示不旋转

    /* 加载位图，默认RGB888 */
    error = FT_Load_Char(face, code, FT_LOAD_RENDER);//加载face.glyph
    if (error) {
        printf("FT_Load_Char error\n");
        return -1;
    }
    slot = face->glyph;

    fbt->buffer = slot->bitmap.buffer;

    fbt->region.x = slot->bitmap_left;
    fbt->region.y = fbt->currOriginY * 2 - slot->bitmap_top;
    fbt->region.width = slot->bitmap.width;
    fbt->region.height = slot->bitmap.rows;
    fbt->nextOriginX = fbt->currOriginX + slot->advance.x / 64;
    fbt->nextOriginY = fbt->currOriginY;

    return 0;
}


static Font_style freetype_style = {
        .name = "freetype",
        .font_init = freetype_font_init,
        .font_set_size = freetype_set_size,
        .font_get_bitmap = freetype_get_bitmap,
};

void freetype_font_register(void)
{
    register_font_style(&freetype_style);
}