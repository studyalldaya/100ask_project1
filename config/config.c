//
// Created by LONG on 2022/10/3.
//
#include <stdio.h>
#include <string.h>
#include "../include/config.h"

static Item_config itemConfigs[ITEM_CONFIG_MAX_NUMBER];//保存每一行
static int itemConfigCount = 0;

int parse_config_file(void)
{
    FILE *fp;
    char buf[100];
    char *p = NULL;
    /*open config file*/
    //fp = fopen(fileName, "r");
    fp = fopen(CONFIG_FILE, "r");
    // fp = fopen((fileName ? fileName : CONFIG_FILE), "r");
    if (!fp) {
        printf("can not open config file %s\n", CONFIG_FILE);
        return -1;
    }
    /*读取每一行*/
    while (fgets(buf, 100, fp)) {
        /*忽略开头的空格或TAB*/
        p = buf;
        while (*p == ' ' || *p == '\t')
            p++;
        /*忽略注释 # */
        if (*p == '#')
            continue;
        /*处理每一行*/
        itemConfigs[itemConfigCount].command[0] = '\0';//判断是否读到command
        itemConfigs[itemConfigCount].index = itemConfigCount;
        sscanf(p, "%s %d %s", itemConfigs[itemConfigCount].name, &itemConfigs[itemConfigCount].can_touch,
               itemConfigs[itemConfigCount].command);
        itemConfigCount++;
    }
    fclose(fp);
    return 0;
}

int get_itemcfg_count(void)
{
    return itemConfigCount;
}

Item_config *get_itemcfg_by_index(int index)
{
    if (index < itemConfigCount)
        return &itemConfigs[index];
    else
        return NULL;

}


Item_config *get_itemcfg_by_name(char *name)
{
    int i;
    for (i = 0; i < itemConfigCount; i++) {
        if (strcmp(name, itemConfigs[i].name) == 0)
            return &itemConfigs[i];
    }
    return NULL;
}
