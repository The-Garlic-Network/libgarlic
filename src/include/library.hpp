/**
*	library.hpp - Header file of decentralized
*	network TGN. Here is published functions of
*	library.
*
*	@mrrva - 2019
*/
#ifndef TGN_LIBRARY
#define TGN_LIBRARY
/**
*	Included header files.
*/
#include "encryption.hpp"
#include "database.hpp"
#include "packages.hpp"
#include "network.hpp"
#include "nodes.hpp"
#include "tgn.hpp"
/*
*	Functions' prototypes.
*/
extern "C" {
	void tgn_callback(void (*callback)(char *, unsigned char *));
	bool tgn_send_msg(const char *, size_t, unsigned char *);
	void the_garlic_network(void);
	const char *tgn_myhash(void);
	bool tgn_is_working(void);
	void tgn_restart(void);
	void tgn_stop(void);
}

#endif