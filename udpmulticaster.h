

#ifndef PUBSUB_H
#define PUBSUB_H

#include <map>
#include <set>
#include "PracticalSocket.h"

using namespace std;

class udpmulticaster
{
public:
	udpmulticaster(const UDPSocket& sock): sock(sock){};
	~udpmulticaster(){};
	
	void subscribe(SocketAdress adr, string topic){
		if (subscriptions.count(topic) == 0)
			subscriptions[topic] = set<SocketAdress>;
		subscriptions[topic].insert(adr);
	}
	
	void unsubscribe(SocketAdress adr, string topic){
		subscriptions[topic].erase(adr);
	}
	void send(const void* buffer, int bufferlen, string topic){
		for (std::set<TCPSocket*>::iterator it = subscribe[topic].begin(); it != subscriptions[topic].end(); ++it)
        sock.sendTo(buffer, bufferlen, *it);
	}

private:
	map<string, set<SocketAdress> > subscriptions;
	UDPSocket& sock;

};

#endif // PUBSUB_H
