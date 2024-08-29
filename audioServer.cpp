#include<iostream>
#include<bits/stdc++.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#define SIZE 1024
using namespace std;


void serveClient(int socket)
{
    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    while (1)
    {
        int sockinfo = accept(socket, (struct sockaddr *)&cliaddr, &clilen);
        if (sockinfo < 0)
        {
            exit(0);
        }
        cout << "Socket accepted" << endl;
        char accessToken[1024];
        char sz[10];
        recv(sockinfo, accessToken, 1024, 0);
        cout << "Access token of client:"<<accessToken << endl;
        
        struct stat file_stats;
        stat("sample.mp3", &file_stats);
        sprintf(sz,"%d",(int)file_stats.st_size);
        send(sockinfo,sz,10,0);

        FILE *filepointer = fopen("sample.mp3", "rb");
        if (!strcmp(accessToken,"PQR"))
        {
            int s;
            char filedata[1024] = {0};
            char msg[512];
            while ((s=fread(filedata, sizeof(char) , 1024, filepointer))>0)
            {
                if (send(sockinfo, filedata, s, 0) == -1)
                {
                    exit(1);
                }
                bzero(filedata, SIZE);
            }   
            cout<<"Your File is sent successfully";
            sockinfo = -1;    
                      
        }
        else
        {   
            char state[1024] = "Incorrect Token :Access Denied";
            cout << "Incorrect Token" << endl;
            send(sockinfo, state, 1024, 0);
            sockinfo = -1;
            continue; 
        }
        
        fclose(filepointer);
    }
}
int main()
{   
    int portno = 6000;
    char bootstrapIp[] = {'1','2','7','.','0','.','0','.','1'};
    char buffer[SIZE];
    bzero(buffer,SIZE);
    char ip[30];
    int port;
    char servicename[50];
    char type[10];
    char accessToken[15];
    
    int socketInfo=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socketInfo < 0) 
        cerr<<"Error occured while socket creation";
    //servers like servicename, IP address, port number, and a service access token on UDP.
    
    struct sockaddr_in serverAddress;
    bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(bootstrapIp);
    serverAddress.sin_port = htons(portno);

    cout<<"This is a AUDIO Server."<<endl;
    buffer[strlen(buffer)]='S';buffer[strlen(buffer)]='E';buffer[strlen(buffer)]='R';buffer[strlen(buffer)]='V';buffer[strlen(buffer)]='I';buffer[strlen(buffer)]='C';buffer[strlen(buffer)]='E';
    buffer[strlen(buffer)]=':';buffer[strlen(buffer)]='A';buffer[strlen(buffer)]='U';buffer[strlen(buffer)]='D';buffer[strlen(buffer)]='I';buffer[strlen(buffer)]='O';
    buffer[strlen(buffer)]=' ';buffer[strlen(buffer)]='T';buffer[strlen(buffer)]='Y';buffer[strlen(buffer)]='P';buffer[strlen(buffer)]='E';buffer[strlen(buffer)]=':';
    cout<<"Sevice type of this server(extension of file) :";
    cin>>buffer+strlen(buffer);
    buffer[strlen(buffer)]=' ';buffer[strlen(buffer)]='I';buffer[strlen(buffer)]='P';buffer[strlen(buffer)]=':';
    cout<<"Give a static ip address :";
    cin>>buffer+strlen(buffer);
    buffer[strlen(buffer)]=' ';buffer[strlen(buffer)]='P';buffer[strlen(buffer)]='O';buffer[strlen(buffer)]='R';buffer[strlen(buffer)]='T';buffer[strlen(buffer)]=':';
    cout<<"Service port number :6996";
    buffer[strlen(buffer)]='6';buffer[strlen(buffer)]='9';buffer[strlen(buffer)]='9';buffer[strlen(buffer)]='6';
    buffer[strlen(buffer)]=' ';buffer[strlen(buffer)]='T';buffer[strlen(buffer)]='O';buffer[strlen(buffer)]='K';buffer[strlen(buffer)]='E';buffer[strlen(buffer)]='N';buffer[strlen(buffer)]=':';
    cout<<"\nAnd the access token to this server is :PQR";
    buffer[strlen(buffer)]='P';buffer[strlen(buffer)]='Q';buffer[strlen(buffer)]='R';
    buffer[strlen(buffer)]=' ';

    //sprintf(buffer, "%s %s %d %s %s %s", "TEXT_SERVICE will be provide on", ip, port, type,"with access token ",accessToken);
    
    cout<<buffer;
    int c=sendto(socketInfo,"*",50,0,(struct sockaddr *) &serverAddress, sizeof(serverAddress));
    if (c < 0) 
        cerr<<"Error occured while sending data to bootstrap server";
    
    int a=sendto(socketInfo,buffer,strlen(buffer),0,(struct sockaddr *) &serverAddress, sizeof(serverAddress));
    if (a < 0) 
        cerr<<"Error occured while sending data to bootstrap server";
    
    bzero(buffer,SIZE);
  
    cout<<"\nThe server is done with registaring to bootstrap server";   

    cout<<"\nServer is waiting for client ...."<<endl;
    socketInfo = -1;

    int Info = socket(AF_INET, SOCK_STREAM, 0);
    if (Info < 0)
    {
        cout << "Error in creating socket" << endl;
        exit(0);
    }
    cout << "Socket created successfully" << endl;
    // socket binding

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(6996);
    if (bind(Info, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        cout << "Error in binding" << endl;
        exit(0);
    }
    cout << "Bind Successfully " << endl;

    listen(Info, 10);
    cout << "Audio server is listening on port 6996...." << endl;
    
    thread service[9];
    for(int i=0;i<9;i++)
    {   
        service[i]=thread(serveClient, Info);
    }
    for(int i=0;i<9;i++)
    {
        service[i].join();
    }

    return 0;
    }
    
  