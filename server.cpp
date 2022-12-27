#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>
#include <thread>

//#include "user.hpp"

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT   27015


void* func(void* args){
    char client_message[DEFAULT_BUFLEN];
    
    //Receive a message from client
    int* client_sock = (int*)args;
    recv(*client_sock , client_message , DEFAULT_BUFLEN , 0);
    puts(client_message);
}

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    pthread_t threads[3];
   
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(DEFAULT_PORT);

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    int thread_num = 0;

    while(1){
        //Listen
        listen(socket_desc , 3);

        //Accept and incoming connection
        puts("Waiting for incoming connections...");
        c = sizeof(struct sockaddr_in);

        //accept connection from an incoming client
        client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
        if (client_sock < 0)
        {
            perror("accept failed");
            return 1;
        }
        puts("Connection accepted");

        pthread_create(&threads[thread_num],NULL,func,&client_sock);
        thread_num++;
    }

    return 0;
}