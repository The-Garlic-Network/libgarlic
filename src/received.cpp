/**
*	received.cpp - Source file of decentralized
*	network TGN.
*
*	@mrrva - 2019
*/
#include "include/received.hpp"
/**
*	Needed namespaces.
*/
using namespace std;
/**
*	_received::operator << - Will be called when was
*	reseived new package.
*
*	@message - Package.
*/
void _received::operator <<(tgnmsg message)
{
	char hash[HASHSIZE * 2 + 1];
	unsigned char *pack, *buff;
	struct tgn_garlic info;
	size_t length = 0;
	string hex;

	if (!this->function) {
		return;
	}

	info = message.info_garlic();
	buff = message.garlic_msg();
	hex = bin2hex<HASHSIZE>(info.from);

	pack = tgnencryption.unpack(buff);
	strcpy(hash, hex.c_str());
	memcpy(&length, pack, 2);

	if (length <= MAXINPUT - 2) {
		this->function(hash, pack + 2, length);
	}

	delete[] pack;
	delete[] buff;
}
/**
*	_received::set_callback - Callback function
*	setting.
*
*	@callback - Pointer to function.
*/
bool _received::set_callback(
	void (*callback)(char *, unsigned char *, size_t))
{
	if (!callback || callback == nullptr) {
		return false;
	}

	this->function = callback;
	return true;
}