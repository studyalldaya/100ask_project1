//
// Created by LONG on 2022/9/30.
//

#ifndef INC_100ASK_PROJECT1_UI_H
#define INC_100ASK_PROJECT1_UI_H

#include "../include/common.h"
#include "../include/disp_manager.h"
#include "../include/input_manager.h"

#define BUTTON_DEFAULT_COLOR 0xff0000
#define BUTTON_CLICKED_COLOR 0x00ff00
#define BUTTON_TEXT_COLOR    0x000000
struct Button;
typedef int (*On_draw_ptr)(struct Button *btn, Display_buffer *buffer);
typedef int (*On_clicked_ptr)(struct Button *btn, Display_buffer *buffer, Input_data *inputData);
typedef struct Button {
    char *name;
    int status;
    Region btn_region;
    On_draw_ptr on_draw;
    On_clicked_ptr on_clicked;

} Button;

void init_button(Button *btn, char *name, Region *region, On_draw_ptr on_draw, On_clicked_ptr on_clicked);
#endif //INC_100ASK_PROJECT1_UI_H
