/*
Authors:Chris Rawlings and Tyler Ryan  
Date:2013.11.12
Class:CS284
File:server.cpp
Purpose:server side of chat room
*/

#include <iostream>
#include <pthread.h> 
#include <cstdlib>
#include <stdio.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>  /* define socket */
#include <netinet/in.h>  /* define internet socket */
#include <netdb.h>       /* define internet socket */
using namespace std;

#define SERVER_PORT 7777

const int MAX_CLIENT = 10;
int FD[MAX_CLIENT];
//char usernames[MAX_CLIENT][512];
int counter = -1;
pthread_mutex_t m;

void* runClient(void* arg);

int main()
{
  int sd;
  struct sockaddr_in server_addr = { AF_INET, htons( SERVER_PORT ) };
  struct sockaddr_in client_addr = { AF_INET };
  unsigned int client_len = sizeof( client_addr );
  
  for(int i = 0; i < MAX_CLIENT; i++)
  {
    FD[i] = 0;
  }
  /* create a stream socket */
  if( ( sd = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 )
  {
    cerr << "server: socket failed" << endl;
    exit( 1 );
  }
  
  /* bind the socket to an internet port */
  if( bind(sd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1 )
  {
    cerr << "server: bind failed" << endl;
    exit( 1 );
  }

  /* listen for clients */
  if( listen( sd, MAX_CLIENT-1 ) == -1 )
  {
    cerr << "server: listen failed" << endl;
    exit( 1 );
  }

  cout << "SERVER is listening for clients to establish a connection\n";

  int temp;
  while((temp = accept(sd, (struct sockaddr*)&client_addr, &client_len )) > 0)
  {  
    pthread_mutex_lock(&m);
    FD[counter++] = temp;   
    pthread_mutex_unlock(&m);
    pthread_t clientThread;
    pthread_create(&clientThread, NULL, runClient, &temp);
  }
  
  
  //close stuff ...


 
 return 0;
}

void* runClient(void* arg) 
{
  if(arg == NULL)
  {
    cerr << "Oh no! Null argument" << endl; // TODO: Make professional
  }
  
  char buffer[512];
  int k;
  int skt = *(int *)arg;
  int location;
  for(int i = 0; i < MAX_CLIENT; i++)
  {
    if(FD[i] == skt)
    {
      location = i;
    }
  }
  char username[500];
  char message[1024];
  //get  fd;
  //read in client name;
  if((k = read(skt, buffer, sizeof(buffer))) > 0)
  {   
    cout << buffer << " joined" << endl;
    strcpy(username, buffer);
    //buffer = "Welcome " + usernames[location];
    //string welcome = "Welcome ";
    
    //buffer = welcome.c_str();
    strcat(message, "Welcome ");
    strcat(message, username);
    
    //print a message about the new client;
    write(skt, message, sizeof(message));
  }
  //write message to each FD
  pthread_mutex_lock(&m);
  //buffer = "Client " + usernames[location] + " has joined the chatroom";
  //buffer = "Client ";
  strcpy(message, "Client ");
  strcat(message, username);
  strcat(message, " has joined the chatroom");
  //cout << "Client " << username << " has joined the chatroom" << endl;
  for(int i = 0; i < MAX_CLIENT; i++)
  {
    if(FD[i] > 0 && FD[i] != skt)
    {
      write(FD[i], message, sizeof(message));
    }
  }
  pthread_mutex_unlock(&m);
    
  while ((k = read(skt, buffer, sizeof(buffer))) > 0)
  {
    pthread_mutex_lock(&m);
    //buffer = usernames[location] + ": " + buffer;
    //string message = buffer;
    strcpy(message, username);
    //buffer = username;
    strcat(message, ": ");
    strcat(message, buffer);
    for(int i = 0; i < MAX_CLIENT; i++)
    {
      if(FD[i] > 0 && FD[i] != skt)
      {
        write(FD[i], message, sizeof(message));
      }
    }
      //write message to each FD
    pthread_mutex_unlock(&m);       
  }

  pthread_mutex_lock(&m);
  FD[location] = 0;
  cout << "He died" << endl;
  pthread_mutex_unlock(&m);
  pthread_exit(arg);
  return NULL;
}
