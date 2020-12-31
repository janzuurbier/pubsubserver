

#ifndef PUBSUB_H
#define PUBSUB_H

#include <map>
#include <set>
#include "PracticalSocket.h"

using namespace std;

struct classcomp {
  bool operator() (const SocketAddress& lhs, const SocketAddress& rhs) const   {
	  if(lhs.getAddress() == rhs.getAddress())
			return lhs.getPort() < rhs.getPort();
		else
			return lhs.getAddress() < rhs.getAddress();
	  
  }
};

class udpmulticaster
{
	
private:
	map<string, set<SocketAddress, classcomp> > subscriptions;
	UDPSocket& sock;
	
public:
	udpmulticaster(UDPSocket& sock): sock(sock){}
	~udpmulticaster(){}
	
	void subscribe(SocketAddress adr, string topic){
		if (subscriptions.count(topic) == 0) {
			set<SocketAddress, classcomp> myset;
			subscriptions[topic] = myset;
		}
		subscriptions[topic].insert(adr);
	}
	
	void unsubscribe(SocketAddress adr, string topic){
		subscriptions[topic].erase(adr);
	}
	void send(const void* buffer, int bufferlen, string topic){
		for (std::set<SocketAddress, classcomp>::iterator it = subscriptions[topic].begin(); it != subscriptions[topic].end(); ++it)
        sock.sendTo(buffer, bufferlen, *it);
	}



};

#endif // PUBSUB_H
