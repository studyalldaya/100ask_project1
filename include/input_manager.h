//
// Created by LONG on 2022/9/26.
//

#ifndef INC_100ASK_PROJECT1_INPUT_MANAGER_H
#define INC_100ASK_PROJECT1_INPUT_MANAGER_H

#define INPUT_TYPE_TOUCH 1
#define INPUT_TYPE_NET   2
typedef struct input_data {
    struct timeval time;
    int type;//touch or net?
    int x;
    int y;
    int presure;
    char *str[1024];//from net
} Input_data;

struct input_device {
    char *name;

    int (*device_init)(void);//open
    int (*device_exit)(void);//close
    int (*get_input_data)(Input_data *inputData);

    struct input_device *next;
};

#endif //INC_100ASK_PROJECT1_INPUT_MANAGER_H
