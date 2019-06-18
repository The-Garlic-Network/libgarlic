
#ifndef TGN_LIB
#define TGN_LIB

#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <mutex>
#include <map>
#include <string>
#include <cstring>
#include <sstream>
#include <sqlite3.h>
#include <sodium.h>

#define TGN_DEBUG 1;
/**
*	Константы проекта.
*/
const short HEADERSIZE	= 500;
const short TEXTSIZE	= 9000;
const short HASHSIZE	= 32;
const short FULLSIZE	= TEXTSIZE + HEADERSIZE;
const short INFOSIZE	= HEADERSIZE - HASHSIZE - 1;
const short PORT		= 2121;
const short TIMEOUT		= 6;
/**
*	Глобальные шаблоны и функции.
*/
template<short L>
unsigned char bytes_sum(unsigned char *B)
{
	unsigned char summ = 0x00;
	short i;

	if (!B || B == nullptr)
		return summ;

	for (i = 0; i < L; i++)
		summ += B[i];
	return summ;
}

template<short S>
std::string bin2hex(unsigned char *bytes)
{
	char key[TEXTSIZE];

	if (S > TEXTSIZE)
		return "";

	memset(key, 0x00, TEXTSIZE);
	sodium_bin2hex(key, TEXTSIZE, bytes, S);
	return std::string(key);
}

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

inline std::string ipfrombytes(unsigned char *b)
{
	std::stringstream ss;
	int sum = 0;

	if (!b || b == nullptr)
		return "";

	for (size_t i = 0; i < 4; i++) {
		if (b[i] < 0 || b[i] > 254)
			return "";

		sum += static_cast<int>(b[i]);
	}

	if (sum == 0 || b[0] == 0x00)
		return "";

	ss << static_cast<int>(b[0]) << "."
		<< static_cast<int>(b[1]) << "."
		<< static_cast<int>(b[2]) << "."
		<< static_cast<int>(b[3]);

	return ss.str();
}

inline unsigned char *iptobytes(std::string ip)
{
	std::istringstream ss(ip);
	typedef unsigned char uc;
	unsigned char *bytes;
	std::string token;
	size_t i = 0, tmp;

	if (ip.length() < 6)
		return nullptr;

	bytes = new unsigned char[4];

	while (std::getline(ss, token, '.')) {
		if (i > 4)
			break;

		if ((tmp = std::stoi(token)) > 254) {
			delete[] bytes;
			return nullptr;
		}

		bytes[i++] = static_cast<uc>(tmp);
	}

	return bytes;
}

#endif