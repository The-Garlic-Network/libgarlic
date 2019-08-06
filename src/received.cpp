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

void _received::operator <<(tgnmsg message)
{
	/*unsigned char *b_hash = message.byte_key();
	const size_t length = HASHSIZE * 2;
	string hex_hash = hex2bin<length>(b_hash);
	const char *hash, *text;

	hash = hex_hash.c_str();*/
	cout << "Received!!!\n";
}

bool _received::set_callback(
	void (*callback)(const char *, const char *))
{
	if (!callback || callback == nullptr) {
		return false;
	}

	this->function = callback;
	return true;
}