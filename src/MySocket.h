#ifndef MYSOCKET_H
#define MYSOCKET_H


// MySocket.h esta definida para ser utilizada por client.cpp

class MySocket;
/* libreria por defecto c++, equivale a java.io // cout */ 
#include <iostream>

// c library for stdio.h (printf, scanf)
#include <cstdio> // include <stdio.h> //stdout
#include <string.h>

/* connect, create socket and send */
#include <sys/types.h>
#include <sys/socket.h>

/* for errors catching */
#include <errno.h>

/* read from socket */
#include <unistd.h> 

#include <arpa/inet.h>


using namespace std;

class MySocket {
private:
  int    socket_fd;
  struct sockaddr_in server;
  
public:
  MySocket() {
    /* nothing here */
  }

  MySocket(char *ip, int port) throw (int){ /* errno */
    setIp(ip);
    setPort(port);
    setFamily();
    createSocket();
    connectSocket();
  }

  /* server connection configuration methods */

  void setIp(char *ip) {server.sin_addr.s_addr = inet_addr(ip);}
  
  void setPort(int port) {server.sin_port = htons(port);}
  
  void setFamily() {server.sin_family = AF_INET;} //ipv4

  int getSocketfd() {return socket_fd;}

  //create, connect, send and receive methods

  void createSocket() throw (int) {
    if ((socket_fd = socket(AF_INET , SOCK_STREAM , 0)) < 0)
      throw errno;
  }

  void connectSocket() throw (int) {
    if (connect(socket_fd, (struct sockaddr *)&server, sizeof(server)) < 0) //mirar si es necesario el cast
      throw errno;
  }

  void print(string msg) throw (int) {
    if (send(MySocket::socket_fd, msg.c_str(), strlen(msg.c_str()), 0) < 0)  // MySocket::socket_fd equivale a MySocket.socket
      throw errno;
  }
  
  void println(string msg) throw (int) {
    print(msg + "\n");
  }

  //returns size of string (linux$man read)
  int readLine(char* recvBuff) throw (int) {
    memset(recvBuff, '0', sizeof(recvBuff)); // initialise buffer to all zeros
    int n = read(this->socket_fd, recvBuff, sizeof(recvBuff)-1); /*puntero implicito*/
    if (n < 0) //error at reading
      throw errno;
    // n = 0 --> EOF
    recvBuff[n] = 0; //end of file at length n
    return n;
  }
  
  void close() throw (int){
    // close(2) -> closes socket within two direcctions
    if (shutdown(socket_fd,0) < 0)
      throw errno;
    //if (close(socket_fd) < 0) //no funciona
  }
};

#endif
