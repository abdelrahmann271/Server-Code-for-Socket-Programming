#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <winsock.h>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include "http_parser.h"
#include <thread>
using std::ofstream;
using namespace std;
#define MYPORT 4000
#define DEFAULT_BUFLEN 8196
//cout<<"==>"<<AY_5ARA<<"\n";

struct thread_data
{
    int new_fd;
    thread_data(int id) : new_fd(id) {}
};
DWORD WINAPI handle_conncetion(LPVOID lpParameter)
{
    thread_data *td = (thread_data*)lpParameter;
    cout << "thread with socket = " << td->new_fd << endl;
    int new_fd = td->new_fd;
    cout<<"==>"<<"Connection is accepted"<<"\n";

    //Receive
    char buffer[DEFAULT_BUFLEN]={0};
    int iResult, iSendResult;
    int recvbuflen = DEFAULT_BUFLEN;

    //Receive until the peer shuts down the connection
//    do {
//        cout<<"\nlesa\n";
//        iResult = recv(new_fd, buffer, recvbuflen, 0);
//        if (iResult > 0) {
//            printf("Bytes received: %d\n", iResult);
//
////                // Echo the buffer back to the sender
////                iSendResult = send(ClientSocket, recvbuf, iResult, 0);
////                if (iSendResult == SOCKET_ERROR) {
////                    printf("send failed: %d\n", WSAGetLastError());
////                    closesocket(ClientSocket);
////                    WSACleanup();
////                 g   return 1;
////                }
////                printf("Bytes sent: %d\n", iSendResult);
////                iResult = shutdown(new_fd, 2);
////                if (iResult == SOCKET_ERROR) {
////                    printf("shutdown failed: %d\n", WSAGetLastError());
////                    closesocket(sockfd);
////                    WSACleanup();
////                    return 1;
////                }
//        }
//        else if (iResult == 0)
//            printf("Connection closing...\n");
//        else {
//            printf("recv failed: %d\n", WSAGetLastError());
//            closesocket(new_fd);
//            WSACleanup();
//            //return 1;
//        }
//
//    } while (iResult > 0);


        int bReceieved;
        if (   ( bReceieved = recv(new_fd, buffer, DEFAULT_BUFLEN, 0)  ) == -1)
            perror("recv");

    cout<<"==>"<<"Message Received"<<"\n";


    //Reading Buffer.
    cout<<"==>"<<"The messgage"<<"\n";
    //cout<< buffer <<"\n";
    for(int i = 0 ; i < DEFAULT_BUFLEN; i++ ){
        cout<<buffer[i];
    }
    unordered_map<string,string> headers;
    string method;
    string file_name;
    string http_version;
    string body = "";
    parse_http( buffer, headers, method, file_name, http_version,body,size_of_message( buffer));
//        cout<<"==>"<<"method\n";
//        cout<<method<<"\n";
//        cout<<"==>"<<"file_name\n";
//        cout<<file_name<<"\n";
//        cout<<"==>"<<"http_version\n";
//        cout<<http_version<<"\n";

//        cout<<"==>"<<"body\n";
//        cout<<body<<"\n";
//        cout<<"==>"<<"Content-Length\n";
//        cout<<headers["Content-Length"]<<"\n";


    if( method=="GET" ){
        //cout<<"==>"<<"GET"<<"\n";
        char buffer_of_file[2000];
        std::ifstream input;
        input.open(file_name,ios::in);
        HTTP response;
        response.set_status(200);

        string body = "";
        char ch;
        string line = "";
        int size_of_file_to_send = 0;
        //Reading the file into buffer
        while (1) {
            input >>  std::noskipws  >> ch;
            body+=ch;
            size_of_file_to_send++;
            if (input.eof())
                break;
            if(size_of_file_to_send==2000){
                //create a new buffer
            }
        }
        response.add_header("Content-Length", to_string(size_of_file_to_send));
        response.add_body(body);
        string to_Send = response.build();
        int i = 0;
        for( ; i <to_Send.size() ; i++){
            buffer_of_file[i] = to_Send[i];
        }
//            cout<<"\ndebug1\n";
        if (send(new_fd,  buffer_of_file, i, 0) == -1)
            perror("send");
//            cout<<"\nGET is sent!!!\n";
    }
    else if( method =="POST" ){
//            cout<<"POST"<<"\n";
        string type = headers["Content-Type"];
        if(type == "text/plain"){
            ofstream MyFile;
            string createFile = "";
            string path=file_name;
            createFile = path + "/" + "SAMPLE_FILENAME" + ".txt";
            MyFile.open(createFile.c_str());
            if(!MyFile){
                cout<<"==>"<<"Error Writing to File\n";
            }
            else{
                // Write to the file
                MyFile << body;
                cout<<"==>"<<"File posted Succesully\n";
                // Close the file
                MyFile.close();
                HTTP response;
                response.set_status(200);
                string to_Send =  response.build();
                char buffer_of_file[DEFAULT_BUFLEN];
                int i = 0;
                for( ; i <to_Send.size() ; i++){
                    buffer_of_file[i] = to_Send[i];
                }
                if (send(new_fd,  buffer_of_file, i, 0) == -1)
                    perror("send");
            }
        }
        else if(type == "text/html"){
            ofstream MyFile;
            string createFile = "";
            string path=file_name;
            createFile = path + "/" + "SAMPLE_FILENAME" + ".html";
            MyFile.open(createFile.c_str());
            if(!MyFile){
                cout<<"==>"<<"Error Writing to File\n";
            }
            else{
                // Write to the file
                cout<<"==>"<<"File posted Succesully\n";
                MyFile << body;
                // Close the file
                MyFile.close();
            }
            HTTP response;
            response.set_status(200);
            string to_Send =  response.build();
            char buffer_of_file[DEFAULT_BUFLEN];
            int i = 0;
            for( ; i <to_Send.size() ; i++){
                buffer_of_file[i] = to_Send[i];
            }
            if (send(new_fd,  buffer_of_file, i, 0) == -1)
                perror("send");
        }
        else{
            ofstream MyFile;
            string createFile = "";
            string path=file_name;
            createFile = path + "/" + "SAMPLE_FILENAME" + ".jpg";
            MyFile.open(createFile.c_str(),std::ofstream::binary | std::ofstream::out);
            if(!MyFile){
                cout<<"==>"<<"Error Writing to File\n";
            }
            else{
                // Write to the file
//                    cout<<body;
                //MyFile << body;
                char buf[body.size()];
                cout<<"\nbody\n"<<body.size()<<"\n";
                for(int i = 0 ; i < body.size() ; i++){
                    buf[i] = body[i];
                    cout<<buf[i];
                }
//                MyFile.write(buf,body.size());
                MyFile<<body;
                cout<<"==>"<<"File posted Succesully\n";
                // Close the file
                MyFile.close();
            }
            HTTP response;
            response.set_status(200);
            string to_Send =  response.build();
            char buffer_of_file[DEFAULT_BUFLEN];
            int i = 0;
            for( ; i <to_Send.size() ; i++){
                buffer_of_file[i] = to_Send[i];
            }
            if (send(new_fd,  buffer_of_file, i, 0) == -1)
                perror("send");
        }
    }
    else{
        //error not get or post!
    }
    closesocket(new_fd);
    return 0;
}


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
        cout<<"hehe1";
        //Accept
        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd = accept(sockfd, (struct sockaddr *) &their_addr,
                             &sin_size)) == -1) {
            perror("accept");
            continue;
        }
        cout<<"hehe2";
        CreateThread(NULL, 0, handle_conncetion, new thread_data(new_fd) , 0, 0);

    }
#pragma clang diagnostic pop
}



