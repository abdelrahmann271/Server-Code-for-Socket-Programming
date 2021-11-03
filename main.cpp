#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <winsock.h>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
using std::ofstream;
using namespace std;
#define MYPORT 4000
//cout<<"==>"<<AY_5ARA<<"\n";

vector<char *> get_lines(char *buffer){
    vector<char *> lines;
    char *line = strtok(buffer, "\n");
    lines.push_back(line);
    while (line != NULL)
    {
        //printf("%s\n", line);
        line = strtok(NULL, "\n");
        lines.push_back(line);
    }
    return lines;
}

vector<char *> get_words(char * line){
    vector<char *> words;
    char *word = strtok(line, " ");
    words.push_back(word);
    while (word != NULL)
    {
        //printf("%s\n", line);
        word = strtok(NULL, " ");
        words.push_back(word);
    }
    return words;
}

int main(int argc, char *argv[]) {

    //Initialization.

    WSAData wsaData;
    if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed.\n");
        exit(1);
    }

    //Socket.

    int sockfd, new_fd;
    struct sockaddr_in my_addr; // my address information
    struct sockaddr_in their_addr; // connector’s address information
    int sin_size;
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // do some error checking!
    my_addr.sin_family = AF_INET; // host byte order
    my_addr.sin_port = htons(MYPORT);; // choose an unused port at random
    my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");; // use my IP address
    memset(&(my_addr.sin_zero), '\0', 8); // zero the rest of the struct

    //Bind

    int b = bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr));

    //Listen
    int l = listen(sockfd, 5);

    //Accept and Receive ..

    while (1) {

        //Accept
        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd = accept(sockfd, (struct sockaddr *) &their_addr,
                             &sin_size)) == -1) {
            perror("accept");
            continue;
        }

        //Receive

        char buffer[8196]={0};
        if (recv(new_fd, buffer, 8196, 0) == -1)
            perror("recv");

        //Reading Buffer.

        cout<<buffer<<"\n";
        vector<char *> lines = get_lines(buffer);
        vector<char *> words = get_words(lines[0]);
        //Check for error.
        char *type_of_request = words[0];
        char *file_name = words[1];

        cout<<"Type of the request\n";
        if( strcmp(type_of_request,"GET") == 0 ){
            cout<<"==>"<<"GET"<<"\n";
        }
        else if( strcmp(type_of_request,"POST") == 0 ){
            cout<<"POST"<<"\n";
        }
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
            int c = 19;
            while (1) {
                my_file >> ch;
                buffer_of_file[c] = ch;
                c++;
                if (my_file.eof())
                    break;

                cout << ch;
                if(c==2000){
                    //create a new buffer
                }
            }


            if (send(new_fd,  buffer_of_file, 2000, 0) == -1)
                perror("send");
        }
        my_file.close();



        closesocket(new_fd);
    }
}



