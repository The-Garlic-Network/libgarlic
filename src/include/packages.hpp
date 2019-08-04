/**
*	packages.hpp - Header file of decentralized
*	network TGN. Here is published class of 
*	packages processing.
*
*	@mrrva - 2019
*/
#ifndef TGN_PACKAGES
#define TGN_PACKAGES
/**
*	Included header files.
*/
#include "tgn.hpp"
#include "message.hpp"
#include "nodes.hpp"
/**
*	Main class of the module.
*/
class _packages {
	private :
		std::vector<struct tgn_garlic> msgs;
		std::vector<struct tgn_pack> packs;
		std::mutex mute;

		bool remove_one(enum tgn_htype);

	public :
		void new_garlic(unsigned char *, unsigned char *, size_t);
		struct tgn_pack garlic_to_pack(struct tgn_garlic &);
		time_point<system_clock> first_ping(void);
		void system_request(enum tgn_htype);
		void add_bytes(unsigned char *);
		void nodes_response(tgnmsg &);
		struct tgn_pack get_one(void);
		void garlic_status(tgnmsg &);
		void ping_packages(void);
		void resp_msg(tgnmsg &);
		size_t garlic_size(void);
		void add_msg(tgnmsg);
		size_t size(void);
};
/**
*	Object of module controlling.
*/
inline _packages tgnpacks;

#endif