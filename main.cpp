#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <winsock.h>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include "http_parser.h"
using std::ofstream;
using namespace std;
#define MYPORT 4000
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

    while (1) {

        //Accept
        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd = accept(sockfd, (struct sockaddr *) &their_addr,
                             &sin_size)) == -1) {
            perror("accept");
            continue;
        }
        cout<<"==>"<<"Connection is accepted"<<"\n";

        //Receive

        char buffer[8196]={0};

        if (recv(new_fd, buffer, 8196, 0) == -1)
            perror("recv");
        cout<<"==>"<<"Message Received"<<"\n";

        //Reading Buffer.
        cout<<"==>"<<"The messgage"<<"\n";
        cout<<buffer<<"\n";

        unordered_map<string,string> headers;
        string method;
        string file_name;
        string http_version;
        string body;
        parse_http(buffer, headers, method, file_name, http_version,body,size_of_message(buffer));

        cout<<"==>"<<"method\n";
        cout<<method<<"\n";
        cout<<"==>"<<"file_name\n";
        cout<<file_name<<"\n";
        cout<<"==>"<<"http_version\n";
        cout<<http_version<<"\n";
        cout<<"==>"<<"body\n";
        cout<<body<<"\n";


        if( method=="GET" ){
            //cout<<"==>"<<"GET"<<"\n";
            fstream my_file;
            char buffer_of_file[2000];
            my_file.open(file_name, ios::in);
            if (!my_file) {
                cout << "No such file";
                if (send(new_fd, "HTTP/1.1 404 Not Found\r\n\r\n", 19, 0) == -1)
                    perror("send");
            }
            else {
                string firstLine = "HTTP/1.1 200 OK\r\n\r\n";
                for(int i = 0 ; i < 19 ; i++){
                    buffer_of_file[i] = firstLine[i];
                }
                char ch;
                int size_of_file_to_send = 19;
                while (1) {
                    my_file >> ch;
                    buffer_of_file[size_of_file_to_send] = ch;
                    size_of_file_to_send++;
                    if (my_file.eof())
                        break;

                    //cout << ch;
                    if(size_of_file_to_send==2000){
                        //create a new buffer
                    }
                }
                if (send(new_fd,  buffer_of_file, size_of_file_to_send, 0) == -1)
                    perror("send");
            }
            my_file.close();
        }
        else if( method =="POST" ){
            cout<<"POST"<<"\n";
            cout<<headers["Content-Type"]<<"\n";
            cout<<file_name<<"\n";

            ofstream MyFile(file_name+"/filew5alas.txt");
            if(!MyFile){
                cout<<"==>"<<"Error Writing to File\n";
            }
            else{
                // Write to the file
                cout<<"==>"<<"File posted Succesully\n";
                MyFile << "Files can be tricky, but it is fun enough!";

                // Close the file
                MyFile.close();
            }



        }
        else{
            //error not get or post!
        }
        closesocket(new_fd);
    }
}



