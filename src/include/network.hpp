/**
*	network.hpp - Header file of decentralized
*	network TGN. Here is published class of socket
*	and processing of packages.
*
*	@mrrva - 2019
*/
#ifndef TGN_NETWORK
#define TGN_NETWORK
/**
*	Included header files.
*/
#include "tgn.hpp"
#include "received.hpp"
#include "packages.hpp"
#include "message.hpp"
#include "nodes.hpp"
/**
*	Main class of the module.
*/
class _network {
	private :
		struct sockaddr_arr sddr_r, sddr_s;
		struct timeval timeout;
		int sock;

		void set_options(int &);
		void thread_send(void);
		void thread_resv(void);
		bool start(void);

	public :
		std::thread th_s, th_r;
		std::atomic<bool> stop;

		void process_request(unsigned char *);
		bool current_node(struct sockaddr_in &);
		void set_node(struct tgn_node);
		void switching_node(void);
		void wait_threads(void);
		~_network(void);
		_network(void);	
};
/**
*	Additional functions for the module.
*/
inline struct tgn_ipport ipport_get(struct sockaddr_in a)
{
	struct tgn_ipport ipport;
	char tmp[21];

	inet_ntop(AF_INET, &a.sin_addr, tmp, 20);
	ipport.port = ntohs(a.sin_port);
	ipport.ip = std::string(tmp);

	return ipport;
}

inline struct sockaddr_in saddr_get(std::string ip, short p)
{
	struct sockaddr_in sockaddr;

	if (ip.length() < 5)
		return sockaddr;

	sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());
	sockaddr.sin_port = htons(p);
	return sockaddr;
}
/**
*	Object of module controlling.
*/
inline _network tgnnetwork;

#endif