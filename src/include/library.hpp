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
	void tgn_callback(void (*callback)(char *, unsigned char *, size_t));
	size_t tgn_send_msg(char *, size_t, unsigned char *);
	enum tgn_status tgn_status(size_t);
	bool tgn_resend_package(size_t);
	void tgn_remove_packages(void);
	void the_garlic_network(void);
	bool tgn_is_working(void);
	void tgn_restart(void);
	char *tgn_myhash(void);
	void tgn_stop(void);
}

#endif