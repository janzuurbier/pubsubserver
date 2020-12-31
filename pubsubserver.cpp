#include <iostream>
#include "PracticalSocket.h"
#include "udpmulticaster.h"
using namespace std;




int main(int argc, char *argv[]) {
	if(argc != 3) {
		cout << "missing arguments:  pubsubserver <ipadres> <port> "  << endl;
		return -1;
	}
	const char* adres = argv[1];
	string portstring = argv[2];
	int port = stoi(portstring);
	try {
		
		SocketAddress localadr (adres, port, SocketAddress::UDP_SOCKET);
		UDPSocket sock;
		sock.bind(localadr);
		udpmulticaster themulticaster(sock);
		
		
		cout << " waiting...." << endl;
		char buffer[128];
		SocketAddress remoteadr;
 
  
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
