//
// Created by LONG on 2022/9/29.
//
#include <freetype2/ft2build.h>

#include "../include/font_manager.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H

static FT_Face face;
static int defaultFontSize = 12;

static int freetype_font_init(char *fname)
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
    ret = FT_New_Face(library, fname, 0, &face); /* create face object */
    if (ret) {
        printf("FT_New_Face err\n");
        return -1;
    }
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
    FT_GlyphSlot slot = g_tFace->glyph;

    pen.x = fbt->currOriginX * 64; /* 单位: 1/64像素 */
    pen.y = fbt->currOriginY * 64; /* 单位: 1/64像素 */

    /* 转换：transformation */
    FT_Set_Transform(face, 0, &pen);

    /* 加载位图: load glyph image into the slot (erase previous one) */
    error = FT_Load_Char(face, code, FT_LOAD_RENDER);
    if (error) {
        printf("FT_Load_Char error\n");
        return -1;
    }

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