/*
Authors:Tyler Ryan and Chris Rawlings
Date:2013.11.12
Class:CS284
File:client.cpp
Purpose:client side of chat room

buff[0] = '\0'; strcpy(buff, "");
*/

#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>

#define SERVER_PORT 7777

using namespace std;

void signalHandler(int signalNumber)
{
  if(signalNumber==2)
  {
    cout<<"To exit type /quit /exit or /part"<<endl;
  }
  
}

int main(int argc, char* argv[])
{
  signal(SIGINT, signalHandler);
  int clientSocket;
  bool exitCondition=false;
  char buff[1024];
  char quit[]="/quit";
  char exitb[]="/exit";
  char part[]="/part";
  char code[]="bRZUkq3h173Uc31";
  struct hostent *hostServer;
  struct sockaddr_in server_addr = { AF_INET, htons( SERVER_PORT ) };

  //checks to see if the correct number of arguments were passed in
  if(argc != 2)
  {
    cout<<"Usage: "<<argv[0]<<" hostname"<<endl;
    exit(1);
  }

  //checks to see if the hostname is valid
  if( (hostServer = gethostbyname(argv[1]) ) == NULL)
  {
    cout<<"host "<<argv[1]<<" not found"<<endl;
    exit(1);
  }
  
  memcpy( hostServer->h_addr_list[0], (char*)&server_addr.sin_addr, hostServer->h_length );
    
  //creating a socket for the  client
  if( (clientSocket=socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    cout<<"Socket Creation Failure"<<endl;
    exit(1);
  }
  
  //client connecting to a socket
  if( (connect( clientSocket,(struct sockaddr*)&server_addr, sizeof(server_addr))) == -1) 
  {
    cout<<"Connection Failed"<<endl;
    exit(1);
  }
  
  cout<<"Connection Successful"<<endl;
  cout<<"Enter Username: ";
  cin>>buff;
  write(clientSocket, buff, sizeof(buff));
  //strcpy(buff, "");
  
  while(exitCondition==false)
  {
    cin>>buff;
    if(strcmp(buff, quit)==0 || strcmp(buff, exitb)==0 || strcmp(buff, part)==0)
    {
      exitCondition=true;
      exit(1);
    }
    else
    {
      write(clientSocket, buff, sizeof(buff));
      read(clientSocket, buff, sizeof(buff));
      if(strcmp(buff, code) == 0)
      {
        exitCondition=true;
        cout<<"The Server is shutting down"<<endl;
        cout<<"This program will terminate in 10 seconds"<<endl;
        sleep(10000);
        cout<<"Exiting now"<<endl;
        exit(1);
      }
      cout<<buff<<endl;
    }
  }
  
  close(clientSocket);
  return 0;
}
