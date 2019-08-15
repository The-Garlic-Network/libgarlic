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
	const size_t len = HASHSIZE * 2;
	unsigned char *b_hash, *pack, *buff, pack_fn[TEXTSIZE];
	char hash[len + 1];
	size_t length = 0;
	string hex;

	if (!this->function) {
		return;
	}

	b_hash = message.byte_key();
	buff = message.garlic_msg();
	hex = bin2hex<len>(b_hash);
	pack = tgnencryption.unpack(buff);

	memcpy(pack_fn, pack + 2, MAXINPUT - 2);
	strcpy(hash, hex.c_str());
	memcpy(&length, pack, 2);

	this->function(hash, pack_fn, length);

	delete[] b_hash;
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