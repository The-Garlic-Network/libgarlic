/**
*	garlic.h - Header file of functions prototypes.
*	This header file is for libgarlic.so.
*
*	@mrrva - 2019
*/
#ifndef LIB_GARLIC
#define LIB_GARLIC
/**
*	Constant from sodium.h
*/
#define crypto_box_SEALBYTES 48
/**
*	Constant of maximum acceptable length of package
*	for tgn_send_msg function.
*/
#define MAXINPUT 9000 - crypto_box_SEALBYTES - 2
/**
*	Constant of hash size.
*/
#define HASHSIZE 32
/**
*	tgn_status - Structure of package statuses.
*/
enum tgn_status {
	NEW_PACKAGE		= 0xff,
	DOESNT_EXISTS	= 0xfe,

	EMPTY_STATUS	= 0x00,
	REQUEST_FIND	= 0x01,

	GOOD_SERVER		= 0x02,
	GOOD_TARGET		= 0x03,

	ERROR_ROUTE		= 0x04,
	ERROR_TARGET	= 0x05
};
/**
*	tgn_callback - Function setting for calling when
*	new package was received.
*
*	@argument 1 - Pointer to function.
*/
void tgn_callback(void (*callback)(char *, unsigned char *, size_t));
/**
*	the_garlic_network - Initialization of the
*	project.
*/
void the_garlic_network(void);
/**
*	tgn_myhash - Returns user hash (public key) in
*	hex format.
*/
char *tgn_myhash(void);
/**
*	tgn_is_working - Getting status of the network 
*	threads.
*/
bool tgn_is_working(void);
/**
*	tgn_restart - Restart threads. Call the function
*	when need to start working after calling tgn_stop.
*/
void tgn_restart(void);
/**
*	tgn_stop - Stop all threads. Call the function
*	when need to stop working.
*/
void tgn_stop(void);
/**
*	tgn_send_msg - Function of sending byte packages
*	to any client of the network.
*
*	@argument 1 - Hash of the network client.
*	@argument 2 - Length of package.
*	@argument 3 - Byte package.
*/
size_t tgn_send_msg(char *, size_t, unsigned char *);
/**
*	tgn_pack_status - Getting status of sent package.
*
*	@argument 1 - Package id.
*/
enum tgn_status tgn_pack_status(size_t);
/**
*	tgn_resend_package - Trying to resend exists package
*	from the list of garlic packages.
*
*	@argument 1 - Package id.
*/
bool tgn_resend_package(size_t);
/**
*	tgn_remove_packages - Removing all packs in the list
*	of garlic packages.
*/
void tgn_remove_packages(void);

#endif
