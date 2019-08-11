
#include <iostream>
#include <sodium.h>
#include <cstring>

using namespace std;
const short TEXTSIZE = 9000;
const short HASHSIZE = 32;



template<short S>
unsigned char *hex2bin(std::string line)
{
	unsigned char *buff;
	size_t len;

	if (line.length() % 2 != 0)
		return nullptr;

	len = S / 2 + 1;
	buff = new unsigned char[len];

	sodium_hex2bin(buff, len, line.c_str(),
		line.length(), NULL, NULL, NULL);
	return buff;
}

template<short L>
void print_bytes(unsigned char *B)
{
	if (!B || B == nullptr)
		return;

	for (short i = 0; i < L; i++)
		std::cout << "0x" << std::hex << + B[i] << " ";
}




string public_key_hex = "3d7f0c925c70e739241b7643e2d3f9dd329cb80e7e1f9dceb4012418dd732359";
string secret_key_hex = "a04b0e304167da1a011035f2be46e335e7db997d7e61743e58904cf6e1bca86e";
string text_s("Hello world!");

int main()
{


	const size_t limit = TEXTSIZE - crypto_box_SEALBYTES;
/////////////////////////////////////
	const size_t hs = HASHSIZE * 2;
	const char *text = text_s.c_str();
	unsigned char *b_text = new unsigned char[limit];
	unsigned char *buffer = new unsigned char[TEXTSIZE];
	unsigned char *public_key = hex2bin<hs>(public_key_hex);
	unsigned char *secret_key = hex2bin<hs>(secret_key_hex);





	memset(buffer, 0x00, TEXTSIZE);
	memset(b_text, 0x00, limit);
	memcpy(b_text, text, strlen(text) + 1);


	print_bytes<limit>(b_text);
	cout << endl << "==================" << endl;


	crypto_box_seal(buffer, b_text, limit, public_key);
	


	unsigned char *buf2 = new unsigned char[limit];


	if (crypto_box_seal_open(buf2, buffer, TEXTSIZE,
		public_key, secret_key) != 0) {
		cout << "[E] crypto_box_seal_open in "
			<< "_encryption::unpack\n";
		exit(1);
	}

	//print_bytes<limit>(buf2);
	char *decr = new char[limit];
	memcpy(decr, buf2, limit);
	cout << string(decr) << endl;
}