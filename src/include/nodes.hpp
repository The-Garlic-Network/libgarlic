
#ifndef TGN_NODES
#define TGN_NODES

#include "tgn.hpp"
#include "database.hpp"

struct tgn_node {
	unsigned char hash[HASHSIZE];
	std::string ip;
};

class _nodes {
	private :
		std::vector<struct tgn_node> nodes;

	public :
		void new_node(struct tgn_node);
		struct tgn_node get_one(void);
		void remove_node(std::string);
		bool set_node(std::string);
		void set_nodes(void);
};

inline _nodes tinnodes;

#endif