//
// Created by LONG on 2022/9/29.
//
#include <stdio.h>
#include <string.h>

#include "../include/font_manager.h"

static Font_style *fontStyleHead = NULL;
static Font_style *defaultFontStyle = NULL;

extern void freetype_font_register(void);

void register_font_style(Font_style *fs)
{
    fs->next = fontStyleHead;
    fontStyleHead = fs;
}

void font_init(void)
{
    freetype_font_register();
}


int select_and_init_font(char *style, char *fname)
{
    Font_style *ptTmp = fontStyleHead;
    while (ptTmp) {
        if (strcmp(ptTmp->name, style))
            break;
        ptTmp = ptTmp->next;
    }

    if (!ptTmp)
        return -1;

    defaultFontStyle = ptTmp;
    return ptTmp->font_init(fname);
}

int font_set_size(int size)
{
    return defaultFontStyle->font_set_size(size);
}

int font_get_bitmap(unsigned int code, Font_bitmap *fbm)
{
    return defaultFontStyle->font_get_bitmap(code, fbm);
}
