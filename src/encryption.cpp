/**
*	encryption.cpp - Source file of decentralized
*	network TGN.
*
*	@mrrva - 2019
*/
#include "include/encryption.hpp"
/**
*	Needed namespaces.
*/
using namespace std;
/**
*	_encryption::new_keys - Generation of pairs for
*	encryption.
*/
void _encryption::new_keys(void)
{
	if (this->public_key) {
		delete[] this->public_key;
	}

	if (this->secret_key) {
		delete[] this->secret_key;
	}

	this->public_key = new unsigned char[HASHSIZE];
	this->secret_key = new unsigned char[HASHSIZE];

	crypto_box_keypair(public_key, secret_key);
}
/**
*	_encryption::~_encryption - Destructor of the class.
*	Freeing of prointers.
*/
_encryption::~_encryption(void)
{
	if (this->public_key) {
		delete[] this->public_key;
	}

	if (this->secret_key) {
		delete[] this->secret_key;
	}
}
/**
*	_encryption::set_keys_hex - Set key pair to 
*	pointers.
*
*	@public_hex - Public key (hex foramt).
*	@secret_hex - Secret key (hex foramt).
*/
void _encryption::set_keys_hex(string &public_hex,
	string &secret_hex)
{
	unsigned char *public_b, *sicret_b;
	const short hs = HASHSIZE * 2;

	if (public_hex.length() != hs
		|| secret_hex.length() != hs) {
		cout << "[I]: Create new key pair.\n";
		this->new_keys();

		this->get_keys_hex(public_hex, secret_hex);
		return;
	}
	
	public_b = hex2bin<hs>(public_hex);
	sicret_b = hex2bin<hs>(secret_hex);

	this->set_keys(public_b, sicret_b);
}
/**
*	_encryption::set_keys - Set key pair to class'
*	pointers.
*
*	@public_hex - Public key (byte format).
*	@secret_hex - Secret key (byte format).
*/
void _encryption::set_keys(unsigned char *public_b,
	unsigned char *secret_b)
{
	if (this->public_key) {
		delete[] this->public_key;
	}

	if (this->secret_key) {
		delete[] this->secret_key;
	}

	this->public_key = public_b;
	this->secret_key = secret_b;
}
/**
*	_encryption::set_keys - Getting key pair from the
*	module in byte format.
*
*	@public_hex - Public key.
*	@secret_hex - Secret key.
*/
void _encryption::get_keys_hex(string &public_hex,
	string &secret_hex)
{
	public_hex = bin2hex<HASHSIZE>(this->public_key);
	secret_hex = bin2hex<HASHSIZE>(this->secret_key);
}
/**
*	_encryption::set_keys - Getting key pair from the
*	module in hex foramt.
*
*	@public_b - Public key.
*	@secret_b - Secret key.
*/
void _encryption::get_keys(unsigned char *public_b,
	unsigned char *secret_b)
{
	public_b = this->public_key;
	secret_b = this->secret_key;
}
/**
*	_encryption::pack - Returns public key.
*/
unsigned char *_encryption::my_hash(void)
{
	return this->public_key;
}
/**
*	_encryption::pack - Encryption of byte array for
*	sending to nodes.
*
*	@text - Pointer to byte array.
*	@key - Public key.
*/
unsigned char *_encryption::pack(unsigned char *text,
	unsigned char *key)
{
	unsigned char *buffer = nullptr;
	size_t len;

	if (!text || !key) {
		cout << "[E] _encryption::pack.\n";
		return nullptr;
	}

	len = TEXTSIZE + 100 + crypto_box_SEALBYTES;
	buffer = new unsigned char[len];
	memset(buffer, 0x00, len);

	crypto_box_seal(buffer, text, TEXTSIZE, key);
	return buffer;
}
/**
*	_encryption::unpack - Decryption of byte array for
*	processing.
*
*	@text - Pointer to byte array.
*/
unsigned char *_encryption::unpack(unsigned char *text)
{
	unsigned char *buffer = nullptr;
	size_t len;

	if (!text) {
		cout << "[E] _encryption::unpack.\n";
		return nullptr;
	}

	len = TEXTSIZE + crypto_box_SEALBYTES;
	buffer = new unsigned char[len];
	memset(buffer, 0x00, len);

	if (crypto_box_seal_open(buffer, text, len,
		this->public_key, this->secret_key) != 0) {
		cout << "[E] crypto_box_seal_open in "
			<< "_encryption::unpack\n";
		return nullptr;
	}

	return buffer;
}