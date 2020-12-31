#include "PracticalSocket.h"
#include <iostream>
#include <thread>
using namespace std;

string naam;
TCPSocket sock;
SocketAddress servaddr("127.0.0.1", 8080);
bool doorgaan = true;


void handleConsole(){
	try{
		string bericht;
		while(doorgaan){
			getline(cin, bericht);
			if(bericht == "stop" ) doorgaan = false;
			bericht = naam + ": " + bericht;
			int len = bericht.size();
			char* lenbuf = new char[2];
			lenbuf[0] = len & 0xff;
			lenbuf[1] = (len >> 8) & 0xff;
			sock.send(lenbuf, 2);
			sock.send(bericht.data(), len);
			delete[] lenbuf;
		}
	
	}
	catch(SocketException &e){
		cout << e.what() << endl;
	}
}

void handleNetwork(){
	char* lenbuffer = new char[2];
	char* buffer = new char[500];
	while(doorgaan) {
	   int n = sock.recvFully(lenbuffer, 2);
       if (n == 0) break;
       int len = lenbuffer[0] + (lenbuffer[1] << 8);
       n = sock.recvFully(buffer, len);
       if (n == 0) break;
	   buffer[len] = '\0';
	   cout << buffer << endl;
	}
	doorgaan = false;
}

int main(int argc, char *argv[]) {
	cout << "Wat is je naam? ";
	cin >> naam;
	sock.connect(servaddr);
	thread consoleThread(handleConsole);
	thread networkThread(handleNetwork);
	consoleThread.join();
	networkThread.join();
	sock.close();
	
}