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
static int socketServer;

//server
static int net_device_init(void)
{
    struct sockaddr_in socketServerAddr;
    int ret;

    socketServer = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == socketServer) {
        printf("socket error!\n");
        return -1;
    }
    socketServerAddr.sin_family = AF_INET;
    socketServerAddr.sin_port = htons(SERVER_PORT);  /* host to net, short */
    socketServerAddr.sin_addr.s_addr = INADDR_ANY;
    memset(socketServerAddr.sin_zero, 0, 8);

    ret = bind(socketServer, (const struct sockaddr *) &socketServerAddr, sizeof(struct sockaddr));
    if (-1 == ret) {
        printf("bind error!\n");
        return -1;
    }


    return 0;
}

static int net_device_exit(void)
{
    close(socketServer);
    return 0;
}

static int net_get_input_data(Input_data *inputData)
{
    struct sockaddr_in socketClientAddr;
    unsigned int addrLen;
    int recvLen;
    char recvBuf[1000];
    addrLen = sizeof(struct sockaddr);
    recvLen = recvfrom(socketServer, recvBuf, 999, 0, (struct sockaddr *) &socketClientAddr, &addrLen);
    if (recvLen > 0) {
        recvBuf[recvLen] = '\0';
        //printf("Get Msg From %s : %s\n", inet_ntoa(SocketClientAddr.sin_addr), recv_buf);
        inputData->type = INPUT_TYPE_NET;
        gettimeofday(&inputData->time, NULL);
        strncpy(inputData->str, recvBuf, 1000);
        inputData->str[999] = '\0';
        return 0;
    } else {
        printf("No client msg!\n");
        return -1;
    }

}

static Input_device net_dev = {
        .name = "net",
        .device_init = net_device_init,
        .device_exit = net_device_exit,
        .get_input_data = net_get_input_data,
};

void net_register(void)
{
    register_input(&net_dev);
}