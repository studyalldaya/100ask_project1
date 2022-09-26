//
// Created by LONG on 2022/9/26.
//

#include <stdio.h>
#include <tslib.h>
#include "../include/input_manager.h"

static struct tsdev *ts;

static int touchscreen_device_init(void)
{
    ts = ts_setup(NULL, 0);
    if (!ts)
    {
        printf("ts_setup err\n");
        return -1;
    }
    return 0;
}

static int touchscreen_device_exit(void)
{
    ts_close(ts);
    return 0;
}

static int touchscreen_get_input_data(Input_data *inputData)
{

}

static struct input_device touchscreen_dev = {
        .name = "touchscreen",
        .device_init = touchscreen_device_init,
        .device_exit = touchscreen_device_exit,
        .get_input_data = touchscreen_get_input_data,
};