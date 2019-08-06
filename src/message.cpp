/**
*	message.cpp - Source file of decentralized
*	network TGN.
*
*	@mrrva - 2019
*/
#include "include/message.hpp"
/**
*	Needed namespaces.
*/
using namespace std;
/**
*	tgnmsg::tgnmsg - Constructor of tgnmsg class.
*
*	@bytes - Byte array.
*/
tgnmsg::tgnmsg(unsigned char *bytes)
{
	this->bytes[0] = 0x00;

	if (bytes == nullptr || bytes[0] == 0x00
		|| bytes[0] > RESPONSE_GARLIC) {
		this->length = 0;
		return;
	}

	this->length = this->length_detect(bytes);
	memcpy(this->bytes, bytes, this->length);
}
/**
*	tgnmsg::to_bytes - Returns package in byte
*	format.
*
*	@len - Size of the byte array.
*/
unsigned char *tgnmsg::to_bytes(size_t &len)
{
	unsigned char *tmp;

	if (this->bytes[0] == 0x00)
		return nullptr;

	tmp = new unsigned char[FULLSIZE];
	memcpy(tmp, this->bytes, FULLSIZE);
	len = this->length;

	return tmp;
}
/**
*	tgnmsg::str_key - Translate public key of package
*	owner to hex format.
*/
string tgnmsg::str_key(void)
{
	unsigned char *bytes;
	string pub_key;

	bytes = this->byte_key();

	if (bytes[0] == 0x00)
		return "";

	pub_key = bin2hex<HASHSIZE>(bytes);

	delete[] bytes;
	return pub_key;
}
/**
*	tgnmsg::header_type - Returns type of the package.
*/
enum tgn_htype tgnmsg::header_type(void)
{
	enum tgn_htype type = INDEFINITE_MESSAGE;
	unsigned char b = this->bytes[0];

	if (b > 0x00 && b <= 0x06) {
		type = static_cast<enum tgn_htype>(b);
	}
	return type;
}
/**
*	tgnmsg::get_info - Returns byte array of
*	additional information in package.
*/
unsigned char *tgnmsg::get_info(void)
{
	size_t move = 1 + HASHSIZE;
	unsigned char *tmp;

	 if (this->bytes[0] == 0x00)
	 	return nullptr;

	tmp = new unsigned char[INFOSIZE];
	memcpy(tmp, this->bytes + move, INFOSIZE);
	return tmp;
}
/**
*	tgnmsg::byte_key - Translate public key of package
*	owner to byte format.
*/
unsigned char *tgnmsg::byte_key(void)
{
	unsigned char *tmp;

	if (this->bytes[0] == 0x00)
		return nullptr;

	tmp = new unsigned char[HASHSIZE];
	memcpy(tmp, this->bytes + 1, HASHSIZE);

	return tmp;
}
/**
*	tgnmsg::length_detect - Automatic detection of
*	package size.
*
*	@buffer - Byte array.
*/
size_t tgnmsg::length_detect(unsigned char *buffer)
{
	if (buffer[0] == 0x05 || buffer[0] == 0x06) {
		return FULLSIZE;
	}
	return HEADERSIZE;
}
/**
*	tgnmsg::create - Creating new packege.
*
*	@type - Type of the package.
*	@msg - Byte array.
*	@hash - To whom to send.
*/
void tgnmsg::create(enum tgn_htype type,
	unsigned char *msg, unsigned char *hash)
{
	unsigned char *key;

	if (type == REQUEST_GARLIC && (msg == nullptr
		|| !msg || !hash || hash == nullptr)) {
		cout << "[W]: Incorrect args in tgnmsg::"
			<< "create.\n";
		return;
	}

	key = tgnencryption.my_hash();
	this->length = HEADERSIZE;

	memcpy(this->bytes, &type, 1);
	memcpy(this->bytes + 1, key, HASHSIZE);

	if (type != REQUEST_GARLIC) {
		return;
	}

	memcpy(this->bytes + HASHSIZE + 1, hash, HASHSIZE);
	memcpy(this->bytes + HEADERSIZE, msg, TEXTSIZE);
	this->length = FULLSIZE;
}
/**
*	tgnmsg::is_garlic - Is package has REQUEST_GARLIC
*	type.
*/
bool tgnmsg::is_garlic(void)
{
	enum tgn_htype type = this->header_type();

	if (bytes[0] == 0x00) {
		return false;
	}

	if (type == REQUEST_GARLIC) {
		return true;
	}

	return false;
}
/**
*	tgnmsg::info_garlic - Function of reading data
*	of garlic routing.
*/
struct tgn_garlic tgnmsg::info_garlic(void)
{
	unsigned char *s_ptr = this->bytes + HASHSIZE + 1;
	unsigned char st = *(s_ptr + HASHSIZE * 2);
	struct tgn_garlic req;

	if (bytes_sum<INFOSIZE>(s_ptr) == 0x00
		|| st > ERROR_TARGET)
		return req;

	req.status = static_cast<enum tgn_status>(st);
	memcpy(req.from, s_ptr + HASHSIZE, HASHSIZE);
	memcpy(req.to, s_ptr, HASHSIZE);

	return req;
}
/**
*	tgnmsg::garlic_msg - Function of reading data from
*	message block.
*/
unsigned char *tgnmsg::garlic_msg(void)
{
	unsigned char *tmp;

	if (this->length != FULLSIZE
		|| this->bytes[0] == 0x00)
		return nullptr;

	tmp = new unsigned char[TEXTSIZE];
	memcpy(tmp, this->bytes + HEADERSIZE, TEXTSIZE);

	return tmp;
}

void tgnmsg::show_garlic(void)
{
	struct tgn_garlic info;

	if (this->length != FULLSIZE
		|| this->bytes[0] == 0x00) {
		cout << "[W]: Incorrect tgnmsg buffer.\n";
		return;
	}

	cout << "[I]: Decoded garlic info.\n";

	switch (this->bytes[0]) {
	case REQUEST_GARLIC:
		cout << "Message type: REQUEST_GARLIC\n";
		break;

	case RESPONSE_GARLIC:
		cout << "Message type: RESPONSE_GARLIC\n";
		break;

	default:
		cout << "[W]: Incorrect tgnmsg buffer.\n";
		return;
	}

	info = this->info_garlic();

	switch(info.status) {
	case NEW_PACKAGE:
		cout << "Garlic type: NEW_PACKAGE\n";
		break;

	case EMPTY_STATUS:
		cout << "Garlic type: EMPTY_STATUS\n";
		break;

	case REQUEST_FIND:
		cout << "Garlic type: REQUEST_FIND\n";
		break;

	case GOOD_SERVER:
		cout << "Garlic type: GOOD_SERVER\n";
		break;

	case GOOD_TARGET:
		cout << "Garlic type: GOOD_TARGET\n";
		break;

	case ERROR_ROUTE:
		cout << "Garlic type: ERROR_ROUTE\n";
		break;

	case ERROR_TARGET:
		cout << "Garlic type: ERROR_TARGET\n";
		break;

	default:
		cout << "[W]: Incorrect garlic type.\n";
		return;
	}
}