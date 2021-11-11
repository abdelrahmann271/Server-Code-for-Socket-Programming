#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <winsock.h>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include "http_parser.h"
#include "file_handler.h"
#include "connection_handler.h"
#include <thread>

using std::ofstream;
using namespace std;
#define MYPORT 4000
#define DEFAULT_BUFLEN 9000
//cout<<"==>"<<AY_5ARA<<"\n";

int main(int argc, char *argv[]) {

    //Initialization.

    WSAData wsaData;
    if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed.\n");
        exit(1);
    }
    cout<<"==>"<<"Initialization Success"<<"\n";

    //Socket.

    int sockfd, new_fd;
    struct sockaddr_in my_addr; // my address information
    struct sockaddr_in their_addr; // connector’s address information
    int sin_size;
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // do some error checking!
    if(sockfd < 0){
        perror("Socket error");
    }
    cout<<"==>"<<"Socket Success"<<"\n";
    my_addr.sin_family = AF_INET; // host byte order
    my_addr.sin_port = htons(MYPORT);; // choose an unused port at random
    my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");; // use my IP address
    memset(&(my_addr.sin_zero), '\0', 8); // zero the rest of the struct

    //Bind

    int b = bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr));
    if(b < 0){
        perror("Bind error");
    }
    cout<<"==>"<<"Bind Success"<<"\n";

    //Listen
    int l = listen(sockfd, 5);
    if(l < 0){
        perror("listen error");
    }
    cout<<"==>"<<"Server is listening"<<"\n";
    //Accept and Receive ..

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (1) {
        cout<<"==>"<<"Waiting For Connection To Accept .. "<<"\n";
        //Accept
        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd = accept(sockfd, (struct sockaddr *) &their_addr,
                             &sin_size)) == -1) {
            perror("accept");
            continue;
        }
        cout<<"==>"<<"Creating a thread for the conncetion .. "<<"\n";
        CreateThread(NULL, 0, handle_conncetion, new thread_data(new_fd) , 0, 0);
    }
#pragma clang diagnostic pop

    return 0;
}



