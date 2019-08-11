/**
*	garlic.h - Header file of functions
*	prototypes. This header file is for
*	libgarlic.so
*
*	@mrrva - 2019
*/
#ifndef LIB_GARLIC
#define LIB_GARLIC
/**
*	Constant of maximum acceptable length of package
*	for tgn_send_msg function.
*/
#define MAXINPUT 9000 - crypto_box_SEALBYTES;
/**
*	tgn_callback - Function setting for calling when
*	new package was received.
*
*	@callback - Pointer to function.
*/
void tgn_callback(void (*callback)(char *, unsigned char *));
/**
*	the_garlic_network - Initialization of the
*	project.
*/
void the_garlic_network(void);
/**
*	tgn_myhash - Returns user hash (public key) in
*	hex format.
*/
const char *tgn_myhash(void);
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
*	@hash - Hash of the network client.
*	@length - Length of package.
*	@msg - Byte package.
*/
bool tgn_send_msg(const char *, size_t, unsigned char *);

#endif
