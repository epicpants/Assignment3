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

void runClient(void* arg);

int main()
{
  int sd;
  struct sockaddr_in server_addr = { AF_INET, htons( SERVER_PORT ) };
  struct sockaddr_in client_addr = { AF_INET };
  int client_len = sizeof( client_addr );
  

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

  cout << "SERVER is listening for clients to establish a connection\n" << endl;

  int temp;
  while((temp = accept(sd, (struct sockaddr*)&client_addr, &client_len )) > 0)
  {  
    pthread_mutex_lock(&m);
    FD[counter++] = temp;   
    pthread_mutex_unlock(&m);
    pthread_t clientThread;
    pthread_create(&clientThread, NULL, runClient, counter);
  }
  
  
  //close stuff ...


 
 return 0;
}

void runClient(int* arg) 
{
  char buf[512], *host;
  int k, ns;
  int location = *arg;
  
  string username = "";
  //get  fd;

  //read in client name;
  while((k = read(ns, buf, sizeof(buf))) > 0)
  {   
    cout << buf << " joined" << endl;
    username = buf;
    //buf = "Welcome " + usernames[location];
    string welcome = "Welcome ";
    buf = welcome.c_str();
    strcat(buf, username);
    //print a message about the new client;
    write(ns, buf, k);
  }
  
  //write message to each FD
  pthread_mutex_lock(&m);
  //buf = "Client " + usernames[location] + " has joined the chatroom";
  buf = "Client ";
  strcat(buf, username);
  strcat(buf, " has joined the chatroom");
  for(int i = 0; i < MAX_CLIENT; i++)
  {
    if(i > 0 && i != location)
    {
      write(FD[i], buf, k);
    }
  }
  pthread_mutex_unlock(&m);
    
  while ((k = read(ns, buf, sizeof(buf))) > 0)
  {
    pthread_mutex_lock(&m);
    //buf = usernames[location] + ": " + buf;
    string message = buf;
    buf = username;
    strcat(buf, ": ");
    strcat(buf, message);
    for(int i = 0; i < MAX_CLIENT; i++)
    {
      if(i > 0 && i != location)
      {
        write(FD[i], buf, k);
      }
    }
      //write message to each FD
    pthread_mutex_unlock(&m);       
  }

  pthread_mutex_lock(&m);
  FD[location] = 0;
  pthread_mutex_unlock(&m);
  pthread_exit(arg);
}
