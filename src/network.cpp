/**
*	network.cpp - Модуль отвечающий за работу
*	с сетью и потоком приема данных.
*
*	@mrrva - 2019
*/
#include "include/network.hpp"
/**
*	Используемые пространства имен и объекты.
*/
using namespace std;
/**
*	_network::~_network - Конструктор класса _network
*	который инициализирует все переменные.
*/
_network::_network(void)
{
	this->sock = socket(AF_INET, SOCK_DGRAM, 0);
	this->s_data.srv.sin_port = htons(PORT);
	this->s_data.srv.sin_family = AF_INET;
	this->stop = false;

	if (this->sock == 0) {
		cout << "[E] _network::_network: start "
			<< "socket.\n";
		exit(1);
	}

	s_data.p_srv = reinterpret_cast<struct sockaddr *>(
		&this->s_data.srv);
	this->set_opts(this->sock);
}
/**
*	_network::~_network - Деструктор класса _network.
*	Уничтожает переменные и указатели.
*/
_network::~_network(void)
{
	this->stop = true;

	if (this->sock != 0) {
		shutdown(this->sock, SHUT_RDWR);
		close(this->sock);
	}
}
/**
*	_network::set_opts - Функция задающая параметры
*	сокету сети.
*
*	@stream - Ссылка на сокет.
*/
void _network::set_opts(int &stream)
{
	int sol = SOL_SOCKET, opt;
	char *t_opt;

	if (stream == 0) {
		cout << "[E] _network::set_opts.\n";
		return;
	}

	t_opt = reinterpret_cast<char *>(&this->timeout);
	this->timeout.tv_sec = TIMEOUT;
	opt = sizeof(this->timeout);
	this->timeout.tv_usec = 0;

	setsockopt(stream, sol, SO_RCVTIMEO, t_opt, opt);
	setsockopt(stream, sol, SO_SNDTIMEO, t_opt, opt);
}
/**
*	_network::set_node - Функция задающая текущую ноду
*	для работы с сетью.
*
*	@ip - Ip ноды.
*/
bool _network::set_node(string ip)
{
	char *i = ip.c_str();

	if (ip.length() < 6) {
		cout << "[E] _network::change_node.\n";
		return false;
	}

	s_data.srv.sin_addr.s_addr = inet_addr(i);

	this->start_receiving();
	this->stop = false;
}