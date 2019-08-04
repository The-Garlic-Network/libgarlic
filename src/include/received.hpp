
#ifndef TGN_RECEIVED
#define TGN_RECEIVED

#include "tgn.hpp"
#include "message.hpp"

class _received {
	private :
		std::vector<tgnmsg> list;
		std::mutex mute;

	public :
		void operator<<(tgnmsg);
};

inline _received received_messages;

#endif