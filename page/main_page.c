//
// Created by LONG on 2022/10/3.
//
#include <stdio.h>
#include "../include/page_manager.h"


static void main_page_run(void *param)
{
    /*读取配置文件*/

    /*根据配置文件生成button和界面*/

    while (1) {
        /*读取输入事件*/
        /*根据输入事件找到对应button*/
        /*执行相应button的on_clicked函数*/
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