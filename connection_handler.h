#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <winsock.h>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <thread>

using std::ofstream;
using namespace std;
#define MYPORT 4000
#define DEFAULT_BUFLEN 9000

struct thread_data
{
    int new_fd;
    thread_data(int id) : new_fd(id) {}
};
DWORD WINAPI handle_conncetion(LPVOID lpParameter)
{
    thread_data *td = (thread_data*)lpParameter;
    cout<<"==>"<<"Thread with socket = "<<td->new_fd <<"\n";
    int new_fd = td->new_fd;
    cout<<"==>"<<"Connection is accepted"<<"\n";

    //This while loop is to receive data until the connection is closed or it is timeout.
    while(1){

        fd_set fds ;
        int n ;
        struct timeval tv ;

        // Set up the file descriptor set.

        FD_ZERO(&fds) ;
        FD_SET(new_fd, &fds) ;

        // Set up the struct timeval for the timeout.

        tv.tv_sec = 5 ;
        tv.tv_usec = 0 ;

        // Wait until timeout or data received.
        cout<<"krkrkr\n";
        n = select ( new_fd, &fds, NULL, NULL, &tv ) ;
        if ( n == 0)
        {
            cout<<"==>"<<"Timeout"<<"\n";
            int iResult = shutdown(new_fd, 2);
            if (iResult == SOCKET_ERROR) {
                printf("shutdown failed: %d\n", WSAGetLastError());
                closesocket(new_fd);
                WSACleanup();
                cout<<"==>"<<"Connection closed due to timeout"<<"\n";
                return 1;
            }
            break;
            //return 0 ;
        }
        else if( n == -1 )
        {
            printf("Error..\n");
            return 1;
        }

        //Receive

//        char buffer[DEFAULT_BUFLEN]={0};
//        int iResult, iSendResult;
//        int bReceived;
//        if (   ( bReceived = recv(new_fd, buffer, DEFAULT_BUFLEN, 0)  ) == -1)
//            perror("recv");
//
//        //Client Closes The connection..
//        if(bReceived == 0){
//            break;
//        }

        char buffer[DEFAULT_BUFLEN];
        int bReceived = -1;
        HTTP request;
        unordered_map<string,string> headers;
        string method;
        string file_name;
        string http_version;
        string body = "";



        if ((bReceived = recv(new_fd,  buffer, DEFAULT_BUFLEN, MSG_PEEK)) > 0) {
            cout<<"size of buffer\n";
            cout<<size_of_message(  buffer)<<"\n";;
//            cout<<bReceived<<"\n";
//            cout<<" debug1\n";
            parse_http(  buffer, headers, method, file_name, http_version,body,size_of_message(  buffer));
//            cout<<"debug2\n";
            int totalBytes = size_of_message(  buffer);
            recv(new_fd, buffer, totalBytes, 0);
            cout<<"buffer receievd\n";
            for(int i = 0 ; i < totalBytes ; i++){
                cout<<buffer[i];
            }
            int bodyLength = std::atoi(headers["Content-Length"].c_str());
            cout<<"content length\n";
            cout<<bodyLength<<"\n";
            cout<<"receievd\n";
            cout<<body.size()<<"\n";
            bodyLength-=body.size();
            //std::string body;
//            cout<<"debug3\n";
            while (bodyLength > 0) {
//                cout<<"debug5\n";
                int read = recv(new_fd, buffer, std::min(DEFAULT_BUFLEN, bodyLength), 0);
                cout<<read<<"\n";
//                cout<<"debug6\n";
                for (int i = 0; i < read; i++)
                {
                    body +=  buffer[i];
                }
                bodyLength -= DEFAULT_BUFLEN;
            }
//            cout<<"debug4\n";
            std::cout << body.size() << std::endl;
            cout<<body;
        }

//        cout<<"==>"<<"Number of bytes Receieved: "<<bReceived<<"\n";
//        cout<<"==>"<<"Message Received:"<<"\n";
//        for(int i = 0 ; i <  bReceived; i++ ){
//            cout<<buffer[i];
//        }
//
////        Reading Buffer and parsing it.
//
//        unordered_map<string,string> headers;
//        string method;
//        string file_name;
//        string http_version;
//        string body = "";
//        parse_http( buffer, headers, method, file_name, http_version,body,size_of_message( buffer));
//        cout<<method;
        if( method=="GET" ){
//            cout<<"debug11\n";
            file_name = "/Users/Abdelrahman Nour/CLionProjects/network" + file_name;
            //char buffer_of_file[2000];
            char buffer_of_file[DEFAULT_BUFLEN];
            std::ifstream input;
            input.open(file_name,ios::in | ios::binary);

            //If the file is not found.

            if(!input){
                cout << "==>" << "File Not Found .. \n";

                HTTP response;
                response.set_status(404);
                string to_Send = response.build();

                //char buffer_of_file[DEFAULT_BUFLEN];
                int i = 0;
                for (; i < to_Send.size(); i++) {
                    buffer_of_file[i] = to_Send[i];
                }
                if (send(new_fd, buffer_of_file, i, 0) == -1)
                    perror("send");
            }

            //If the file is found.

            else{

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

                if (send(new_fd,  buffer_of_file, i, 0) == -1)
                    perror("send");


            }
            cout<<"debug1asdadasd\n";
        }
        else if (method=="POST" ) {
            cout<<"debug22\n";
            char buffer_of_file[DEFAULT_BUFLEN];
            ofstream MyFile;
            string createFile = "";
//            string path = file_name;
            file_name = "/Users/Abdelrahman Nour/CLionProjects/network" + file_name;
            string path = file_name;
            //createFile = path + "/" + "SAMPLE_FILENAME" + ".txt";
            //createFile = path;
            createFile =  path;
            //cout<<"\n"<<path<<"\n";
            MyFile.open(createFile.c_str(), std::ofstream::binary | std::ofstream::out);
            //MyFile.open(createFile.c_str());

            if (!MyFile) {
                cout << "==>" << "File Not Found .. Creating File ..\n";
                //
                int lstInd;
                for (int i = 0; i < file_name.size(); i++) {
                    if (file_name[i] == '/') {
                        lstInd = i;
                    }
                }
                string path = file_name.substr(0, lstInd);
                string file = file_name.substr(lstInd + 1, file_name.size() - lstInd);
                cout << path << " " << file;
                makePath(path);
                createFile = path + "/" + file;
                //

                MyFile.open(createFile.c_str(), std::ofstream::binary | std::ofstream::out);
                MyFile << body;
                cout << "==>" << "File posted Succesully\n";
                // Close the file
                MyFile.close();

                HTTP response;
                response.set_status(200);
                string to_Send = response.build();

                //char buffer_of_file[DEFAULT_BUFLEN];
                int i = 0;
                for (; i < to_Send.size(); i++) {
                    buffer_of_file[i] = to_Send[i];
                }
                if (send(new_fd, buffer_of_file, i, 0) == -1)
                    perror("send");
            }
            else {
                MyFile << body;
                cout << "==>" << "File posted Succesully\n";
                // Close the file
                MyFile.close();

                HTTP response;
                response.set_status(200);
                string to_Send = response.build();
                char buffer_of_file[DEFAULT_BUFLEN];
                int i = 0;
                for (; i < to_Send.size(); i++) {
                    buffer_of_file[i] = to_Send[i];
                }
                if (send(new_fd, buffer_of_file, i, 0) == -1)
                    perror("send");
            }
        }
//        else{
//            cout << "==>" <<"Not Supported Method\n";
//        }
    }


    cout<< "==>" <<"Connection Closed by client..\n";
    cout<< "==>" <<"Closing Socket ..\n";
    closesocket(new_fd);
    return 0;
}