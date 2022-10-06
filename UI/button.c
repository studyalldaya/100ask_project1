//
// Created by LONG on 2022/9/30.
//
#include "../include/UI.h"


static int default_on_draw(struct Button *btn, Display_buffer *buffer)
{
    /*绘制button区域*/
    draw_region(&btn->btn_region, BUTTON_DEFAULT_COLOR);
    /*居中显示文字*/
    draw_text_central(btn->name, &btn->btn_region, BUTTON_TEXT_COLOR);
    /*flush 到 lcd或其它设备*/
    flush_display_region(&btn->btn_region, buffer);
    return 0;
}

static int default_on_clicked(struct Button *btn, Display_buffer *buffer, Input_data *inputData)
{
    unsigned int color = BUTTON_DEFAULT_COLOR;
    btn->status = !btn->status;//每次点击，状态翻转,每次点击都会换颜色
    if (btn->status)
        color = BUTTON_CLICKED_COLOR;
    draw_region(&btn->btn_region, color);
    draw_text_central(btn->name, &btn->btn_region, BUTTON_TEXT_COLOR);
    flush_display_region(&btn->btn_region, buffer);
    return 0;

}

void init_button(Button *btn, char *name, Region *region, On_draw_ptr on_draw, On_clicked_ptr on_clicked)
{
    btn->status = 0;
    btn->name = name;
    if (region)
        btn->btn_region = *region;
    btn->on_draw = on_draw ? on_draw : default_on_draw;
    btn->on_clicked = on_clicked ? on_clicked : default_on_clicked;
}

