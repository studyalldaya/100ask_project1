//
// Created by LONG on 2022/9/26.
//

#ifndef INC_100ASK_PROJECT1_INPUT_MANAGER_H
#define INC_100ASK_PROJECT1_INPUT_MANAGER_H

#include <sys/time.h>
#include "common.h"

#define INPUT_TYPE_TOUCH 1
#define INPUT_TYPE_NET   2
typedef struct Input_data {
    struct timeval time;
    int type;//touchscreen or net?
    int x;
    int y;
    unsigned int presure;
    char str[1024];//from net
} Input_data;

typedef struct Input_device {
    char *name;

    int (*device_init)(void);//open
    int (*device_exit)(void);//close
    int (*get_input_data)(Input_data *inputData);

    struct Input_device *next;
} Input_device;

void register_input(Input_device *dev);

//api
void input_init(void);

int input_device_init(void);

int get_input_data(Input_data *data);

int input_device_exit(void);

#endif //INC_100ASK_PROJECT1_INPUT_MANAGER_H
