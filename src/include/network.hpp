
#ifndef TGN_NETWORK
#define TGN_NETWORK

#include "tgn.hpp"
/**
*	Доступные структуры.
*/
struct sockaddr_arr {
	struct sockaddr_in srv;
	struct sockaddr *p_srv;
};

class _network {
	private :
		struct sockaddr_arr s_data;
		struct timeval timeout;
		int sock;

	public :
		bool stop;

		~_network(void);
		_network(void);
};

#endif