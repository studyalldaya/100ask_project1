//
// Created by LONG on 2022/9/29.
//
#include <ft2build.h>
#include <stdio.h>
#include <wchar.h>
#include "../include/font_manager.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H

static FT_Face face;
static int defaultFontSize = 12;


static int freetype_get_text_bbox(char *str, Cartesian_region *textBBox)
{
    int i;
    int error;
    FT_BBox bbox;
    FT_BBox glyph_bbox;
    FT_Vector pen;
    FT_Glyph glyph;
    FT_GlyphSlot slot = face->glyph;

    /* 初始化 */
    bbox.xMin = bbox.yMin = 32000;
    bbox.xMax = bbox.yMax = -32000;

    /* 指定原点为(0, 0) */
    pen.x = 0;
    pen.y = 0;

    /* 计算每个字符的bounding box */
    /* 先translate, 再load char, 就可以得到它的外框了 */
    for (i = 0; i < strlen(str); i++) {
        /* 转换：transformation */
        FT_Set_Transform(face, 0, &pen);

        /* 加载位图: load glyph image into the slot (erase previous one) */
        error = FT_Load_Char(face, str[i], FT_LOAD_RENDER);
        if (error) {
            printf("FT_Load_Char error\n");
            return -1;
        }

        /* 取出glyph */
        error = FT_Get_Glyph(face->glyph, &glyph);
        if (error) {
            printf("FT_Get_Glyph error!\n");
            return -1;
        }

        /* 从glyph得到外框: bbox */
        FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_TRUNCATE, &glyph_bbox);

        /* 更新外框 */
        if (glyph_bbox.xMin < bbox.xMin)
            bbox.xMin = glyph_bbox.xMin;

        if (glyph_bbox.yMin < bbox.yMin)
            bbox.yMin = glyph_bbox.yMin;

        if (glyph_bbox.xMax > bbox.xMax)
            bbox.xMax = glyph_bbox.xMax;

        if (glyph_bbox.yMax > bbox.yMax)
            bbox.yMax = glyph_bbox.yMax;

        /* 计算下一个字符的原点: increment pen position */
        pen.x += slot->advance.x;
        pen.y += slot->advance.y;
    }

    /* return string bbox */
    textBBox->x = bbox.xMin;
    textBBox->y = bbox.yMax;
    textBBox->width = bbox.xMax - bbox.xMin + 1;
    textBBox->height = bbox.yMax - bbox.yMin + 1;
    return 0;
}


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
        .font_get_text_bbox=freetype_get_text_bbox,
};

void freetype_font_register(void)
{
    register_font_style(&freetype_style);
}