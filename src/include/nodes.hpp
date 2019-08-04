/**
*	nodes.hpp - Header file of decentralized
*	network TGN. Here is published class of 
*	nodes controlling.
*
*	@mrrva - 2019
*/
#ifndef TGN_NODES
#define TGN_NODES
/**
*	Included header files.
*/
#include "tgn.hpp"
#include "database.hpp"
#include "network.hpp"
/**
*	Main class of the module.
*/
class _nodes {
	private :
		std::vector<struct tgn_node> nodes;
		time_point<system_clock> last_req;
		std::mutex mute;

	public :
		time_point<system_clock> last_request(void);
		void remove_sddr(struct sockaddr_in &);
		void add(unsigned char *, std::string);
		void used_sddr(struct sockaddr_in &);
		struct tgn_node get_one(void);
		size_t factual_size(void);
		void used_ip(std::string);
		void db_select(void);
		size_t size(void);
		void ping(void);
		_nodes(void);
};
/**
*	Object of module controlling.
*/
inline _nodes tgnnodes;

#endif