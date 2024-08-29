#include <iostream>
#include<bits/stdc++.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#define SIZE 512

using namespace std;

int main()
{
    socklen_t peerLength;
    char buffer[SIZE];
   
    string data;
    struct sockaddr_in serverAddress, peerAddress;
    int n;
    fstream filePointer;
    int e = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (e < 0) 
        cerr<<"Error occured while socket creation";
    bzero((char *) &serverAddress, sizeof(serverAddress));
    int portno = 6000;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(portno);

    if (bind(e, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) 
        cerr<<"Error occured while binding to socket";

    peerLength = sizeof(peerAddress);
    while(1)
    {   
        bzero(buffer,SIZE);
        n = recvfrom(e,buffer,SIZE,0,(struct sockaddr *) &peerAddress, &peerLength);
        if (n < 0) 
            cerr<<"Error occured while reading data from buffer";
        
        switch (buffer[0])
        {
            case '*':
                cout<<"New server registration :"<<endl;
                
                bzero(buffer,SIZE);
                n = recvfrom(e,buffer,SIZE,0,(struct sockaddr *) &peerAddress, &peerLength);
                if (n < 0) 
                    cerr<<"Error occured while reading data from buffer";
                cout<<buffer<<endl;            
                filePointer.open("serverdata.csv", ios::out | ios::app);
                filePointer << buffer<<'\n';
                filePointer.close();
                break;
            case '#':
                cout<<"Providing info to client"<<endl;
              
                filePointer.open("serverdata.csv", ios::in);
                if (filePointer.is_open())
                {
                    while (filePointer )
                    {   
                        getline (filePointer,data);
                        cout << data << '\n';
                        int b=sendto(e,data.c_str(),data.length(),0,(struct sockaddr *) &peerAddress, peerLength);
                        if (b < 0) 
                            cerr<<"Error occured while sending data to bootstrap server";
                    }
                    filePointer.close();
                    int b=sendto(e,"#",2,0,(struct sockaddr *) &peerAddress, peerLength);
                }
                else
                    cout << "Unable to open file";
                break;
           
           
        }
        
        
    }

}