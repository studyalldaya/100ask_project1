//
// Created by LONG on 2022/10/3.
//

#ifndef INC_100ASK_PROJECT1_CONFIG_H
#define INC_100ASK_PROJECT1_CONFIG_H

#define ITEM_CONFIG_MAX_NUMBER 30
#define CONFIG_FILE "/etc/test_gui/gui.conf"


//config文件的每一行为一个Item_config
typedef struct Item_config {
    int index;
    char name[100];
    int can_touch;//button能否点击
    char command[100];
} Item_config;

int get_itemcfg_count(void);//有多少行

Item_config *get_itemcfg_by_index(int index);//传入index获取该item_config

Item_config *get_itemcfg_by_name(char *name);

#endif //INC_100ASK_PROJECT1_CONFIG_H
