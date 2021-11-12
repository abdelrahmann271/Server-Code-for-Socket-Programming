//
// Created by Abdelrahman Nour on 11/4/2021.
//
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <winsock.h>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
using std::ofstream;
using namespace std;

class HTTP{
    string status;
    unordered_map<string,string> headers;
    string body;

    public:
    HTTP(){

    }
    void set_status(int status_code){
        status = to_string(status_code);
    }
    void add_header(string header_name , string header_value){
        headers[header_name] = header_value;
    }
    void  add_body(string message){
        body = message;
    }
    string build(){
        string http = "";
        http+=("HTTP/1.1 "+status+"\r\n");
        for (auto i = headers.begin(); i != headers.end(); i++){
            http+=i->first+": "+i->second+"\r\n";
        }
        http+="\r\n";
        http+=body;
        return http;
    }


};

int size_of_message(char* buffer){
    int g = 0;
    while(buffer[g] != '\0'){
        g++;
    }
    return g;
}
string convertToString(char* a, int size)
{
    int i;
    string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}

vector<char *> get_lines(char *buffer){
    string s = buffer;
    vector<char *> lines;
    char *line = strtok(buffer, "\r\n");
    //lines.push_back(line);
    while (line != NULL)
    {
        lines.push_back(line);
        line = strtok(NULL, "\r\n");

    }
    return lines;
}

vector<char *> get_words(char * line){
    vector<char *> words;
    char *word = strtok(line, " ");
    //words.push_back(word);
    while (word != NULL)
    {
        words.push_back(word);
        word = strtok(NULL, " ");

    }
    return words;
}

void parse_http(char *message ,
                unordered_map<string,string> &headers ,
                string &method ,
                string &filename ,
                string &http_version ,
                string &body ,
                int size_of_message){




    int i =0;
    int end_of_headers;
    for( ; i < size_of_message ; i++){
        if(message[i] == '\r' && message[i+1] == '\n' && message[i+2] == '\r' && message[i+3] == '\n'){
            end_of_headers = i-1;
            break;
        }
    }
    int k = 0 ;
    char msg[end_of_headers+1];
    while(k!=end_of_headers+1){
        msg[k] = message[k];
        k++;
    }
    msg[k]='\0';
    int j = i+4;

//    int j = i+4;
//    while(message[j] != '\0' ){
//        body+=message[j];
//        j++;
//    }
    vector<char *> lines = get_lines(msg);
    vector<char *> words = get_words(lines[0]);
    method = words[0];
    filename = words[1];
    http_version = words[2];
    char *word;

//    for(int i = 0 ; i < size_of_message ; i++){
//        cout<<message[i];
//    }

    for(int i = 1 ; i < lines.size() ; i++){

        vector<char *> words;
        word= strtok(lines[i], ": ");

        while (word != NULL)
        {

            words.push_back(word);
            word = strtok(NULL, ": ");
        }
        headers[words[0]] = words[1];

    }

    int cnt = ( headers.find("Content-Length") == headers.end() )? 0 : stoi(headers["Content-Length"]);
    //cout<<headers["Content-Length"]<<" content length\n";
    int y = 0;
    while(y<cnt && j < size_of_message){
        body+=message[j];
        j++;
        y++;
    }


//    if(headers.find("Content-Length") != headers.end()){
//        int len = stoi(headers["Content-Length"]);
//        cout<<size_of_message<<" "<<len<<"\n";
//        cout<<"hena 1 \n"<<body<<"\n";
//        for(int i = (size_of_message-len) ; i < size_of_message ; i++){
//            body.push_back(message[i]);
//        }
//        cout<<"hena 2 \n"<<body<<"\n";
//    }



}