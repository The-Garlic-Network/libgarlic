/**
*	nodes.cpp - Модуль отвечающий за работу
*	со списком известных нод.
*
*	@mrrva - 2019
*/
#include "include/nodes.hpp"
/**
*	Используемые пространства имен и объекты.
*/
using namespace std;
/**
*	_nodes::set_nodes - Функция извлечения нод
*	из базы данных в список.
*/
void _nodes::set_nodes(void)
{
	map<string, string> l = tindb.get_nodes();
	struct tgn_node node;
	unsigned char *tmp;

	for (size_t i = 0; i < l.size(); i++) {
		if (l[i].second.length() != HASHSIZE
			|| l[i].first.length() < 6) {
			tindb.remove_node(l[i].first);
			continue;
		}

		tmp = hex2bin<HASHSIZE>(l[i].second);

		if (tmp == nullptr) {
			tindb.remove_node(l[i].first);
			continue;
		}

		memcpy(node.hash, tmp, HASHSIZE);
		node.ip = l[i].first;

		this->nodes.push_back(node);
		delete[] tmp;
	}
}
/**
*	_nodes::remove_node - Удаление ноды из списка
*	и базы данных.
*
*	@ip - Ip адрес ноды.
*/
void _nodes::remove_node(string ip)
{
	vector<struct tgn_node>::iterator it;

	if (this->nodes.size() == 0
		|| ip.length() < 6)
		return;

	it = this->nodes.begin();

	for (; it != this->nodes.end(); it++)
		if ((*it).ip == ip) {
			tindb.remove_node((*it).ip);
			this->nodes.erase(it);
			break;
		}
}
/**
*	_nodes::new_node - Добавление новой ноды в
*	общий список и базу данных.
*
*	@node - Структура ноды.
*/
void _nodes::new_node(struct tgn_node node)
{
	vector<struct tgn_node>::iterator it;
	int cmp = 0, hs = HASHSIZE;
	string hash;

	if (bytes_sum<hs>(node.hash) == 0x00
		|| node.ip.length() < 6)
		return;

	it = this->nodes.begin();

	for (; it != this->nodes.end(); it++) {
		cmp = memcmp((*it).hash, node.hash, hs);

		if (cmp == 0 || (*it).ip == node.ip)
			return;
	}

	hash = bin2hex<HASHSIZE>(node.hash);

	this->nodes.push_back(node);
	tindb.new_node(node.ip, hash);
}
/**
*	_nodes::get_one - Отдает структуру первой
*	ноды из общего списка.
*/
struct tgn_node _nodes::get_one(void)
{
	if (this->nodes.size() == 0) {
		cout << "[E] Node list is empty.\n";
		exit(1);
	}

	return *this->nodes.begin();
}