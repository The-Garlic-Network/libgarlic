/**
*	received.hpp - Header file of decentralized
*	network TGN. Here is published class for 
*	processing received packages.
*
*	@mrrva - 2019
*/
#ifndef TGN_RECEIVED
#define TGN_RECEIVED
/**
*	Included header files.
*/
#include "tgn.hpp"
#include "message.hpp"
/**
*	Main class of the module.
*/
class _received {
	private :
		void (*function)(const char *, const char *);

	public :
		bool set_callback(void (*callback)(const char *, const char *));
		void operator<<(tgnmsg);
};
/**
*	Object of module controlling.
*/
inline _received received_messages;

#endif