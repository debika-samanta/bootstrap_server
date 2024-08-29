#include<iostream>
#include<bits/stdc++.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 512
using namespace std;

int main()
{   
    int bootportno = 6000;
    char bootstrapIp[] = {'1','2','7','.','0','.','0','.','1'};
    char buffer[SIZE];
    char data[SIZE];
    bzero(buffer,SIZE);
    char filename[30];
    char ip[30];
    char accessToken[30];
    int port;
   
    socklen_t addressLength;
    struct sockaddr_in fileservadd;
    int socketInfo=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socketInfo < 0) 
        cerr<<"Error occured while socket creation";
    //servers like servicename, IP address, port number, and a service access token on UDP.
    
    struct sockaddr_in serverAddress;
    addressLength = sizeof(serverAddress);
    bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(bootstrapIp);
    serverAddress.sin_port = htons(bootportno);

    cout<<"Press Any Key to get Data from BOOTSTRAP Server !! :"<<endl;
    cin>>buffer;
    //buffer[strlen(buffer)]=' ';
    int c=sendto(socketInfo,"#",50,0,(struct sockaddr *) &serverAddress, sizeof(serverAddress));
    if (c < 0) 
        cerr<<"Error occured while sending data to bootstrap server";
    int n = recvfrom(socketInfo,data,SIZE,0,(struct sockaddr *) &serverAddress, &addressLength); 
     cout<<data<<endl;
    do{
    n = recvfrom(socketInfo,data,SIZE,0,(struct sockaddr *) &serverAddress, &addressLength); 
    cout<<data<<endl;
    }while( data[0]!='#');
    socketInfo = -1;

    cout<<"\nEnter the File name with extention which will be saved in your system:"<<endl;
    cin>>filename;
    cout<<"\nEnter the IP address of file server:"<<endl;
    cin>>ip;
    cout<<"\nPlease enter the port number of the server from where to access the service :"<<endl;
    cin>>port;
    cout<<"\nPlease enter the access token :"<<endl;
    cin>>accessToken;
    
    FILE *file;
    char bufferdata[1024];    
    file = fopen(filename,"wb+");

    int socktcp = socket(AF_INET, SOCK_STREAM, 0);
    if (socktcp < 0) 
        cerr<<"Error while TCP socket creation :";
    
    bzero((char *) &fileservadd, sizeof(fileservadd));
    fileservadd.sin_family = AF_INET;
    fileservadd.sin_addr.s_addr = INADDR_ANY;
    fileservadd.sin_port = htons(port);

    if(connect(socktcp,(sockaddr *) &fileservadd,sizeof(fileservadd)) < 0) 
        cerr<<"TCP Connection to file server failed";
    
    // createfile(buffer);
    write(socktcp, accessToken, strlen(accessToken));
    char sz[10];
    recv(socktcp, sz, 1024, 0);
    int size = atoi(sz);
   
    while(size > 0 ){
        n = recv(socktcp, bufferdata, sizeof(bufferdata), 0);
        size = size - n;
        fwrite(bufferdata, 1, n, file);
        bzero(bufferdata, 1024);
    }
    fclose(file);
    close(socktcp);
    
    }
    
  