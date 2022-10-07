//
// Created by LONG on 2022/10/3.
//
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "../include/page_manager.h"
#include "../include/config.h"
#include "../include/UI.h"
#include "../include/disp_manager.h"

#define X_GAP 5
#define Y_GAP 5
static Button buttons[ITEM_CONFIG_MAX_NUMBER];
static int n;//button数量

//后续添加以改善文字的显示效果
static int get_font_size_for_all_button(void)
{
    int maxLen = -1;
    int maxIndex;
    int len;
    Cartesian_region cartesianRegion;
    float k, kx, ky;
    /*找出文字最长的button*/
    for (int i = 0; i < n; i++) {
        len = strlen(buttons[i].name);
        if (len > maxLen) {
            maxLen = len;
            maxIndex = i;
        }
    }
    /*以font_size = 100,算出它的bbox框*/
    font_set_size(100);
    font_get_text_bbox(buttons[maxIndex].name, &cartesianRegion);
    /*把文字的bbox缩放为button的bbox(region)*/
    kx = (float) buttons[maxIndex].btn_region.width / cartesianRegion.width;
    ky = (float) buttons[maxIndex].btn_region.height / cartesianRegion.height;
    if (kx < ky)
        k = kx;
    else
        k = ky;
    /*反算出合适的font_size,为保证足够边界，取0.8吧*/
    return k * 100 * 0.8;
}

/*init_button时传入此函数*/
static int main_page_on_clicked(struct Button *btn, Display_buffer *buffer, Input_data *inputData)
{
    unsigned int color = BUTTON_DEFAULT_COLOR;

    char name[100];
    char netInputStatus[100];
    /*最后添加的，支持配置文件的command*/
    char *commandStatus[3] = {"err", "ok", "percent"};
    int commandStatusIndex = 0;//默认err
    char command[1000];
    Item_config *itemConfig;

    char *buttonName = btn->name;//百分比显示的text需要设置
    /*对于touch事件，先判断button是否支持touch事件*/
    if (inputData->type == INPUT_TYPE_TOUCH && inputData->presure) {
        //不支持touch
        if (get_itemcfg_by_name(btn->name)->can_touch == 0)
            return -1;
        /*如果支持touch，变换颜色*/
        btn->status = !btn->status;//每次点击，状态翻转,每次点击都会换颜色
        if (btn->status) {
            color = BUTTON_CLICKED_COLOR;
            commandStatusIndex = 1;//ok
        }
    }
        /*对于net事件，根据传进来的字符串修改相应颜色*/
    else if (inputData->type == INPUT_TYPE_NET) {
        printf("%s %s %d : a net_input\n", __FILE__, __FUNCTION__, __LINE__);
        sscanf(inputData->str, "%s %s", name, netInputStatus);
        if (strcmp(netInputStatus, "ok") == 0) {
            color = BUTTON_CLICKED_COLOR;
            commandStatusIndex = 1;//ok
        } else if (strcmp(netInputStatus, "err") == 0) {
            color = BUTTON_DEFAULT_COLOR;
            commandStatusIndex = 0;//err
        } else if (netInputStatus[0] >= '0' && netInputStatus[0] <= '9') {
            color = BUTTON_PERCENT_COLOR;
            buttonName = netInputStatus;//button text设置为百分比
            commandStatusIndex = 2;
        } else
            return -1;
    } else
        return -1;

    draw_region(&btn->btn_region, color);
    draw_text_central(buttonName, &btn->btn_region, BUTTON_TEXT_COLOR);
    flush_display_region(&btn->btn_region, buffer);

    /*执行command*/
    itemConfig = get_itemcfg_by_name(btn->name);
    if (itemConfig->command[0] != '\0') {
        //构造出新的command 加上ok err percent给sh脚本提供参数,脚本里面使用$1即可得到该参数！
        sprintf(command, "%s %s", itemConfig->command, commandStatus[commandStatusIndex]);
        system(command);//相当于在shell上输入 command,不仅仅只能是.sh
    }
    return 0;

}

static void generate_buttons(void)
{
    int fontSize;
    int width, height;
    int n_per_line;

    int row, rows;//按钮行数
    int col;//列
    Display_buffer *dispBuffer;
    int xres, yres;
    int start_x, start_y;
    int pre_start_x, pre_start_y;
    Button *button;
    int i = 0;
    /*算出单个button的width和height*/
    n = get_itemcfg_count();
    dispBuffer = get_display_buffer();
    xres = dispBuffer->xres;
    yres = dispBuffer->yres;
    width = sqrt(1.0 / 0.618 * xres * yres / n);//浮点运算 xres*yres=width*width*0.618*n
    n_per_line = xres / width + 1;//多显示一个button，再次运算width和height
    width = xres / n_per_line;
    height = 0.618 * width;

    /*居中显示：计算每个button的region*/
    start_x = (xres - width * n_per_line) / 2;//第一个buttond的左顶点x,y
    rows = n / n_per_line;
    if (rows * n_per_line < n) {
        /*rows为小数向下取整，rows+1*/
        rows++;
    }
    start_y = (yres - rows * height) / 2;
    /*计算每个button 的region*/
    for (row = 0; row < rows; row++) {
        pre_start_y = start_y + row * height;
        pre_start_x = start_x - width;//   第一个button的x= （start_x-width） +width
        for (col = 0; (col < n_per_line) && (i < n); col++) {
            button = &buttons[i];//计算buttons里的每一个button的region
            button->btn_region.x = pre_start_x + width;//前面一个button的x+width
            button->btn_region.y = pre_start_y;
            button->btn_region.width = width - X_GAP;
            button->btn_region.height = height - Y_GAP;
            pre_start_x = button->btn_region.x;
            /*init_button*/
            init_button(button, get_itemcfg_by_index(i)->name, NULL, NULL, main_page_on_clicked);
            i++;
        }
    }
    /*适配文字大小*/
    fontSize = get_font_size_for_all_button();
    //font_set_size(fontSize);
    /*ondraw*/
    for (i = 0; i < n; i++) {
        buttons[i].fontSize = fontSize;
        buttons[i].on_draw(&buttons[i], dispBuffer);
    }
}

static int touch_point_in_button_region(int x, int y, Region *region)
{
    if (x < region->x || x >= region->x + region->width)
        return 0;
    if (y < region->y || y >= region->y + region->height)
        return 0;
    return 1;
}

static Button *get_button_by_name(char *name)
{
    int i;
    for (i = 0; i < n; i++) {
        if (strcmp(name, buttons[i].name) == 0)
            return &buttons[i];
    }
    return NULL;
}

static Button *get_button_by_input_data(Input_data *inputData)
{
    int i;
    char name[100];
    if (inputData->type == INPUT_TYPE_TOUCH) {
        for (i = 0; i < n; i++) {
            if (touch_point_in_button_region(inputData->x, inputData->y, &buttons[i].btn_region))
                return &buttons[i];
        }

    } else if (inputData->type == INPUT_TYPE_NET) {
        sscanf(inputData->str, "%s", name);//只提取前一个name字符串，不能用strcpy
        return get_button_by_name(name);

    } else
        return NULL;
    return NULL;
}


static void main_page_run(void *param)
{
    int err;
    Input_data inputData;
    Button *button;
    Display_buffer *displayBuffer = get_display_buffer();
    /*读取配置文件*/
    parse_config_file();

    /*根据配置文件生成button和界面*/
    generate_buttons();

    //printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    while (1) {
        /*读取输入事件*/
        err = get_input_data(&inputData);
        if (err)
            continue;
        //printf("%s %s %d:get_input_data\n", __FILE__, __FUNCTION__, __LINE__);
        /*根据输入事件type找到对应button*/
        button = get_button_by_input_data(&inputData);
        if (!button)
            continue;
        //printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
        /*执行相应button的on_clicked函数,*/
        button->on_clicked(button, displayBuffer, &inputData);
    }
}

static Page main_page = {
        .name = "main",
        .run = main_page_run,
};

void main_page_register(void)
{
    register_page(&main_page);
}