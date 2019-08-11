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
	unsigned char *b_hash, *pack, *buff;
	const size_t len = HASHSIZE * 2;
	char *hash;
	string hex;

	if (!this->function) {
		return;
	}

	b_hash = message.byte_key();
	buff = message.garlic_msg();
	hex = bin2hex<len>(b_hash);
	hash = new char[len + 1];

	pack = tgnencryption.unpack(buff);
	strcpy(hash, hex.c_str());

	this->function(hash, pack);

	delete[] b_hash;
	delete[] buff;
}
/**
*	_received::set_callback - Callback function
*	setting.
*
*	@callback - Pointer to function.
*/
bool _received::set_callback(
	void (*callback)(char *, unsigned char *))
{
	if (!callback || callback == nullptr) {
		return false;
	}

	this->function = callback;
	return true;
}