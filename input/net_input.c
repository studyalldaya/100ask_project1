//
// Created by LONG on 2022/9/26.
//

#include <stdio.h>
#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include "../include/input_manager.h"

#define SERVER_PORT 8888
static int socket_server;

//server
static int net_device_init(void)
{
    struct sockaddr_in SocketServerAddr;
    int ret;

    socket_server = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == socket_server) {
        printf("socket error!\n");
        return -1;
    }
    SocketServerAddr.sin_family = AF_INET;
    SocketServerAddr.sin_port = htons(SERVER_PORT);  /* host to net, short */
    SocketServerAddr.sin_addr.s_addr = INADDR_ANY;
    memset(SocketServerAddr.sin_zero, 0, 8);

    ret = bind(socket_server, (const struct sockaddr *) &SocketServerAddr, sizeof(struct sockaddr));
    if (-1 == ret) {
        printf("bind error!\n");
        return -1;
    }


    return 0;
}

static int net_device_exit(void)
{
    close(socket_server);
    return 0;
}

static int net_get_input_data(Input_data *inputData)
{
    struct sockaddr_in SocketClientAddr;
    unsigned int addr_len;
    int recv_len;
    char recv_buf[1000];
    addr_len = sizeof(struct sockaddr);
    recv_len = recvfrom(socket_server, recv_buf, 999, 0, (struct sockaddr *) &SocketClientAddr, &addr_len);
    if (recv_len > 0) {
        recv_buf[recv_len] = '\0';
        //printf("Get Msg From %s : %s\n", inet_ntoa(SocketClientAddr.sin_addr), recv_buf);
        inputData->type = INPUT_TYPE_NET;
        gettimeofday(&inputData->time, NULL);
        strncpy(inputData->str, recv_buf, 1000);
        inputData->str[999] = '\0';
        return 0;
    } else {
        printf("No client msg!\n");
        return -1;
    }

}

static struct input_device net_dev = {
        .name = "net",
        .device_init = net_device_init,
        .device_exit = net_device_exit,
        .get_input_data = net_get_input_data,
};

void net_register(void)
{
    register_input(&net_dev);
}