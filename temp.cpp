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

int clientSocket;
int k;
char buff[1024];
char buffer[1024];
char code[]="bRZUkq3h173Uc31";
bool exitCondition=false;

void signalHandler(int signalNumber)
{
  if(signalNumber==2)
  {
    cout<<"To exit type /quit /exit or /part"<<endl;
  }
  
}

void * readingOut(void* arg);

int main(int argc, char* argv[])
{
  signal(SIGINT, signalHandler);
  pthread_mutex_t m;
  char quit[]="/quit";
  char exitb[]="/exit";
  char part[]="/part";
  string stupiderStringVar;
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
  read(clientSocket, buff, sizeof(buff));
  cout << buff << endl;

  pthread_t readThread;
  pthread_create(&readThread, NULL, readingOut, &clientSocket); 
  
   cin.ignore(1000, '\n');
   
  while(exitCondition==false)
  { 
    string stupidStringVar;
    cout<<">>";
    getline(cin, stupidStringVar);
    strcpy(buff, stupidStringVar.c_str());
    
    
    if(strcmp(buff, quit)==0 || strcmp(buff, exitb)==0 || strcmp(buff, part)==0)
    {
      exitCondition=true;
      cout << "Exitted!!!" << endl;
      exit(1);
    }
    else
    {
      write(clientSocket, buff, (stupidStringVar.size()));
    }
  }
  strcpy(buff, "");
  
  close(clientSocket);
  return 0;
}

void* readingOut(void* arg)
{
  int clientSocket = *(int *)arg;
  
   //while (( k = (recv(clientSocket, buffer, sizeof(buffer), NULL))) > 0)
  while ((k = read(clientSocket, buffer, sizeof(buffer))) > 0)
  {
    //read(clientSocket, buffer, sizeof(buffer));
    if(strcmp(buffer, code) == 0)
    {
      exitCondition=true;
      cout<<"The Server is shutting down"<<endl;
      cout<<"This program will now terminate"<<endl;
      sleep(1);
      cout<<"Exitted!!!"<<endl;
      exit(1);
    } 
    cout<<buffer<<endl;
    //cout<<">>";
  }
  
  
  return NULL;
}
