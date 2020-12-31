#include <iostream>
#include "PracticalSocket.h"
#include <thread>
#include "udpmulticaster.h"
using namespace std;

udpmulticaster themulticaster;

void handleConnection(TCPSocket* sock, string topic){
   char* buffer = new char[500];
   while(true){
       int n = sock->recvFully(buffer, 2);
       if (n == 0) break;
       int len = buffer[0] + (buffer[1] << 8);
       n = sock->recvFully(buffer + 2, len);
       if (n == 0) break;
       themulticaster.send(buffer, len + 2, topic);
	   buffer[len+2] = '\0';
	   cout << buffer+2 << endl;
   }
   themulticaster.unsubscribe(sock, topic);
   delete buffer;
   delete sock;
}


int main(int argc, char *argv[]) {
	if(argc != 3) {
		cout << "missing arguments:  pubsubserver <ipadres> <port> "  << endl;
		return -1;
	}
	const string adres = argv[1];
	string portstring = argv[2];
	int port = stoi(portstring);
	try {
		SocketAdress localadr (adres, port, SocketAdress::UDP_SOCKET);
		UDPSocket sock;
		sock.bind(localadr);
		cout << " waiting...." << endl;
		char buffer[128];
		SocketAdress remoteadr;
 
  
		for (;;) {                          
			int n = sock.recvFrom(buffer, 128, remoteadr );
			buffer[n] = '\0';
			char *p = buffer;
			while(*p == ' ') p++;
			char *q = p;
			while(*p != ' ') p++;
			*p = '\0';
			p++;
			string type = q;
			while(*p == ' ') p++;
			q = p;
			while(*p != ' ') p++;
			*p = '\0';
			p++;
			string topic = q;
			while(*p == ' ') p++;
			
			
			if(type == "subscribe"){
				themulticaster.subscribe(remoteadr, topic);
				cout << "SUBSCRIBE: " << remoteadr.getAddress() << " TOPIC: "  << topic << endl;
			}
			else if(type == "unsubscribe"){
				themulticaster.unsubscribe(remoteadr, topic);
				cout << "UNSUBSCRIBE: " << remoteadr.getAddress() << " TOPIC: "  << topic << endl;
			}
			else if (type == "publish"){
				themulticaster.send(p, n - (buffer - p), topic);
				cout << "PUBLISH: " << remoteadr.getAddress() << " TOPIC: "  << topic << " MESSAGE: " << p << endl;
			
			}
		}

  } catch (SocketException &e) { cerr << e.what() << endl;  }
  return 0;
}
