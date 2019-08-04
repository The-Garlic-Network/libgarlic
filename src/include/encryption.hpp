/**
*	encryption.hpp - Header file of decentralized
*	network TGN. Here is published class of encryption.
*
*	@mrrva - 2019
*/
#ifndef TGN_ENCRYPTION
#define TGN_ENCRYPTION
/**
*	Included header files.
*/
#include "tgn.hpp"
/**
*	Main class of the module.
*/
class _encryption {
	private :
		unsigned char *public_key, *secret_key;

	public :
		unsigned char *pack(unsigned char *, unsigned char *);
		void set_keys(unsigned char *, unsigned char *);
		void get_keys(unsigned char *, unsigned char *);
		void set_keys_hex(std::string &, std::string &);
		void get_keys_hex(std::string &, std::string &);
		unsigned char *unpack(unsigned char *);
		unsigned char *my_hash(void);
		void new_keys(void);
		~_encryption(void);
};
/**
*	Object of module controlling.
*/
inline _encryption tgnencryption;

#endif