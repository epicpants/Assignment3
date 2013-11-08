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
#include <sys/types.h>
#include <sys/socket.h>  /* define socket */
#include <netinet/in.h>  /* define internet socket */
#include <netdb.h>       /* define internet socket */
using namespace std;

#define SERVER_PORT 7777

const int MAX_CLIENT = 10;
int FD[MAX_CLIENT];
int counter = 0;
pthread_mutex_t m;

void runClient(void* arg);

int main()
{
  int sd, ns, k, pid;
  struct sockaddr_in server_addr = { AF_INET, htons( SERVER_PORT ) };
  struct sockaddr_in client_addr = { AF_INET };
  int client_len = sizeof( client_addr );
  

  /* create a stream socket */
  if( ( sd = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 )
  {
    perror( "server: socket failed" );
    exit( 1 );
  }
  
  /* bind the socket to an internet port */
  if( bind(sd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1 )
  {
    perror( "server: bind failed" );
    exit( 1 );
  }

  /* listen for clients */
  if( listen( sd, MAX_CLIENT-1 ) == -1 )
  {
    perror( "server: listen failed" );
    exit( 1 );
  }

  printf("SERVER is listening for clients to establish a connection\n");

  int temp;
  while((temp = accept(d, (struct sockaddr*)&client_addr, &client_len )) > 0)
  {  
    lock(m);
    FD[counter++] = temp;   
    unlock(m);
    pthread_t clientThread;
    pthread_create(&clientThread, NULL, runClient, temp);
  }

  close stuff ...


 
 return 0;
}

void runClient(void* arg) 
{
  char buf[512], *host;
  int k, ns;
  
  //get  fd;

  //get the host name;
  //read in client name;
  if( (k = read(ns, buf, sizeof(buf))) == -1)
  {    
    cerr << "Server: Read failed" << endl;
  }
  while( (k = read(ns, buf, sizeof(buf))) != 0)
  {   
    printf("SERVER RECEIVED: %s\n", buf);
    write(ns, buf, k);
  }
  print a message about the new client;

  while ((read(FD, buf)) > 0)
  {
    lock(m)
    loop
      write message to each FD
    unlock(m)          
  }

  lock(m);
  remove myself from FDarray
  unlock(m) 
  close(FD)
  thr_exit()
}
