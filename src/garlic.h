/**
*	garlic.h - Header file of functions prototypes.
*
*	@mrrva - 2019
*/
#ifndef LIB_GARLIC
#define LIB_GARLIC
/**
*	Constants.
*/
#define MAXINPUT 9000 - crypto_box_SEALBYTES;
/**
*	Prototypes.
*/
void tgn_callback(void (*callback)(char *, unsigned char *));
bool tgn_send_msg(const char *, size_t, unsigned char *);
void the_garlic_network(void);
const char *tgn_myhash(void);
bool tgn_is_working(void);
void tgn_restart(void);
void tgn_stop(void);

#endif
