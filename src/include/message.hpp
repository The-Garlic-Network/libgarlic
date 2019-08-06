/**
*	message.hpp - Header file of decentralized
*	network TGN. Here is published class of messages.
*
*	@mrrva - 2019
*/
#ifndef TGN_MESSAGE
#define TGN_MESSAGE
/**
*	Included header files.
*/
#include "tgn.hpp"
#include "encryption.hpp"
/**
*	Main class of the module.
*/
class tgnmsg {
	private :
		unsigned char bytes[FULLSIZE];
		size_t length;

		size_t length_detect(unsigned char *);

	public :
		void create(enum tgn_htype, unsigned char *msg = nullptr, unsigned char *hash = nullptr);
		tgnmsg(unsigned char *bytes = nullptr);
		struct tgn_garlic info_garlic(void);
		unsigned char *to_bytes(size_t &);
		unsigned char *garlic_msg(void);
		enum tgn_htype header_type(void);
		unsigned char *get_info(void);
		unsigned char *byte_key(void);
		std::string str_key(void);
		bool is_garlic(void);
		void show_garlic(void);
};

#endif