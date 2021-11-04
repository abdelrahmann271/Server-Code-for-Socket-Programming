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

    vector<char *> lines = get_lines(message);
//    for(int i = 0 ; i < lines.size() ; i++){
//        cout<<lines[i];
//    }
    vector<char *> words = get_words(lines[0]);

    method = words[0];
    filename = words[1];
    http_version = words[2];


    char *word;
    for(int i = 1 ; i < lines.size() ; i++){
        //split on :(space)
        vector<char *> words;
        word= strtok(lines[i], ": ");
        while (word != NULL)
        {
            words.push_back(word);
            //cout<<word<<"\n";
            word = strtok(NULL, ": ");
        }
        headers[words[0]] = words[1];
    }

    int len = stoi(headers["Content-Length"]);
    for(int i = (size_of_message-len) ; i < size_of_message ; i++){
        body.push_back(message[i]);
    }

}