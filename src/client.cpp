/*
  -- general information --
  Filename: client.cpp
  Purpose: connect with a java client
  Author: Gonzalo De Benito
  Date: 2017 May 4

  -- developer information --
 	    
  Linux Compilation: g++ -std=c++11 -pthread client.cpp 
  Linux Execution: ./a.out localhost 2222 Pepito
  Server configuration1: netcat -lp2222
  Server configuration2: java practica2.Server 2222 
  ToDo: ip and port by args 
*/

#include <iostream>
#include <errno.h> //throws
#include <regex> //like java patterns
//#include <string.h> //strncmp
#include <thread>
#include <cstdlib>

#include "MySocket.h"

using namespace std;

void keyboardThread(MySocket ms) {
  // SEND (Keybord Thread) 
  string str_input;
  try {
    while (true) {
      getline(cin, str_input);// cin equivalente a System.in
      // cout << "hello world" enviar
      // cin >> variable_string recibir

      if (cin.eof()) {
	ms.println("DISC");
	break;
      } else {
	ms.println(str_input);
      }
    }
  } catch(int e) {
    cout << strerror(e) << endl;
  }
}

void consoleThread(MySocket ms, string nick) {
  // RECEIVE (Console Thread) 

  char recvBuff[1025];
  int n = 0; //EOF
  //string getnick = "NICK";
  
  try {
    while ((n = ms.readLine(recvBuff)) > 0) {
      
	  
      if (strncmp(recvBuff, "DISC", 4) == 0) {    /* disconnection */
	ms.close();
	break;
      } else if (strncmp(recvBuff, "NICK", 4) == 0) { /* nick request */
	ms.println(nick);
      } else {
	fputs(recvBuff,stdout); //cout << recvBuff; -> no //alternatives: fputs, fwrite or fprintf
      }
    }
  } catch (int e) {
    cout << strerror(e) << endl;
  }
    
}

int main(int argc, char* argv[]) {
  
  try {
    char *ip;
    string nick;
    int port;

    // argument elements == 3
    if (argc <= 3) {
      throw "Usage: executable <ip> <port> <nick>, ex: 127.0.0.1 2222 Pepito";
    }
    
    // ipv4 matching 
    if (std::regex_match(argv[1], std::regex("[0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+"))) {
      ip = argv[1];
    } else if (strcmp(argv[1], "localhost") == 0) {
      ip = (char *) "127.0.0.1";
    } else {
      throw "Bad declaration at IP, try again" ;
    }
      
    // port matching 
    if (std::regex_match(argv[2], std::regex("\\d+") )) {
      port = atoi(argv[2]); //array to integer
    } else {
      throw "Bad declaration at Port, try again";
    }
	
    MySocket ms(ip, port);
    nick = argv[3]; //argument verbose

    thread th_console(consoleThread, ms, nick);
    thread th_keyboard(keyboardThread, ms);

    th_keyboard.join();
    th_console.join();   
      
    return 0;
    
  } catch (int e) {
    cerr << strerror(e) << endl;
    return 1;
    
  } catch (char const* s) {
    cerr << s << endl;
    return 1;
  }
}
