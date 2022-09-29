//
// Created by LONG on 2022/9/28.
//
#include <stdio.h>
#include <pthread.h>
#include "../include/input_manager.h"

/*触摸屏可能一下子会上报很多数据，netdev可能同时有多个client发送数据，
 *所以不能使用单一变量来保存数据，可以使用 --环形缓冲区--   多线程需要访问buffer，访问buffer需要加锁 */
#define BUFFER_LEN 20
static int r = 0;
static int w = 0;
static Input_data inputDatas[BUFFER_LEN];

static int buffer_full(void)
{
    return (r == ((w + 1) % BUFFER_LEN));
}

static int buffer_empty(void)
{
    return (r == w);
}

static void put_data(Input_data *data)
{
    if (!buffer_full()) {
        inputDatas[w] = (*data);
        w = ((w + 1) % BUFFER_LEN);
    }
}

/*数据放入data*/
static int get_data(Input_data *data)
{
    if (!buffer_empty()) {
        (*data) = inputDatas[r];
        r = ((r + 1) % BUFFER_LEN);
        return 0;
    }
    return -1;
}

/*end --环形缓冲区--*/

static Input_device *inputDev = NULL;//链表头
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

extern void touchscreen_register(void);

extern void net_register(void);

void register_input(Input_device *dev)
{
    dev->next = inputDev;
    inputDev = dev;
}

void input_init(void)
{
    /*register touchscreen*/
    touchscreen_register();
    /*register net dev*/
    net_register();
}

//recv data thread
static void *input_recv_thread_func(void *data)
{
    int ret;
    Input_device *dev = (Input_device *) data;
    Input_data inputData;
    while (1) {
        /*读取数据*/
        ret = dev->get_input_data(&inputData);
        if (!ret) {
            /*保存数据*/
            pthread_mutex_lock(&mutex);
            put_data(&inputData);
            pthread_mutex_unlock(&mutex);
            /*唤醒等待数据的线程*/
            pthread_cond_signal(&cond);
        }
    }
}

int input_device_init(void)
{
    int ret;
    pthread_t tid;
    /*对每一个dev ， init and pthread_create*/
    Input_device *temp = inputDev;
    while (temp) {
        /*init dev*/
        ret = temp->device_init();
        /*pthread*/
        if (!ret) {
            /*所有设备开的线程都使用同一个函数，传入temp用来分辨设备*/
            pthread_create(&tid, NULL, input_recv_thread_func, temp);
        }
        temp = temp->next;
    }
    return 0;
}

int input_device_exit(void)
{
    Input_device *temp = inputDev;
    while (temp) {
        temp->device_exit();
        temp = temp->next;
    }
    return 0;
}

/*没有数据就休眠，否则返回数据*/
int get_input_data(Input_data *data)
{
    Input_data dt;
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex);
    if (!get_data(&dt)) {
        //成功得到数据
        (*data) = dt;
        pthread_mutex_unlock(&mutex);
        return 0;
    } else {
        pthread_mutex_unlock(&mutex);
        return -1;
    }
}