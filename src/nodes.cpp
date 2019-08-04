/**
*	nodes.cpp - Source file of decentralized
*	network TGN.
*
*	@mrrva - 2019
*/
#include "include/nodes.hpp"
/**
*	Needed namespaces.
*/
using namespace std;
/**
*	_nodes::_nodes - Constructor of _nodes class.
*/
_nodes::_nodes(void)
{
	this->last_req = system_clock::now();
}
/**
*	_nodes::db_select - Selecting all exist nodes from
*	database with adding to the struct.
*/
void _nodes::db_select(void)
{
	map<string, string> list = tgndb.select_node();
	struct tgn_node node;
	unsigned char *hash;

	for (auto &p : list) {
		hash = hex2bin<HASHSIZE>(p.second);
		memcpy(node.hash, hash, HASHSIZE);
		node.ip = p.first;
		node.used = false;

		this->nodes.push_back(node);

		delete[] hash;
	}
}
/**
*	_nodes::size - Size of unused nodes in the list.
*/
size_t _nodes::size(void)
{
	size_t size = 0;

	this->mute.lock();

	for (auto &p : this->nodes) {
		if (p.used == true) {
			continue;
		}

		size++;
	}

	this->mute.unlock();
	return size;
}
/**
*	_nodes::size - Getting one unused node from the 
*	list.
*/
struct tgn_node _nodes::get_one(void)
{
	struct tgn_node node;

	this->mute.lock();

	for (auto &p : this->nodes) {
		if (p.used == true) {
			continue;
		}

		node = p;
		break;
	}

	this->mute.unlock();
	return node;
}
/**
*	_nodes::remove_sddr - Removing node from the list
*	by sockaddr_in.
*
*	@node - Struct sockaddr_in.
*/
void _nodes::remove_sddr(struct sockaddr_in &node)
{
	struct tgn_ipport ipport = ipport_get(node);
	vector<struct tgn_node>::iterator it;

	this->mute.lock();
	it = this->nodes.begin();

	for (; it != this->nodes.end(); it++) {
		if ((*it).ip != ipport.ip) {
			continue;
		}

		tgndb.remove_node(ipport.ip);
		this->nodes.erase(it);
		break;
	}

	this->mute.unlock();
}
/**
*	_nodes::used_sddr - Set flug used to node by sockaddr_in.
*
*	@node - Struct sockaddr_in.
*/
void _nodes::used_sddr(struct sockaddr_in &node)
{
	struct tgn_ipport ipport = ipport_get(node);
	vector<struct tgn_node>::iterator it;

	this->mute.lock();
	it = this->nodes.begin();

	for (; it != this->nodes.end(); it++) {
		if ((*it).ip == ipport.ip) {
			(*it).used = true;
			break;
		}
	}

	this->mute.unlock();
}
/**
*	_nodes::used_sddr - Set flug used to node by ip
*	address of node.
*
*	@node_ip - Ip address.
*/
void _nodes::used_ip(string node_ip)
{
	vector<struct tgn_node>::iterator it;

	this->mute.lock();
	it = this->nodes.begin();

	for (; it != this->nodes.end(); it++) {
		if ((*it).ip == node_ip) {
			(*it).used = true;
			break;
		}
	}

	this->mute.unlock();
}
/**
*	_nodes::factual_size - Factual size of nodes in
*	the list.
*/
size_t _nodes::factual_size(void)
{
	size_t size = 0;

	this->mute.lock();
	size = this->nodes.size();
	this->mute.unlock();

	return size;
}
/**
*	_nodes::last_request - Getting a time of last request
*	getting node list.
*/
time_point<system_clock> _nodes::last_request(void)
{
	return this->last_req;
}
/**
*	_nodes::ping - Set new time for last_req var.
*/
void _nodes::ping(void)
{
	this->last_req = system_clock::now();
}
/**
*	_nodes::add - Adding new node to the list and
*	to the database.
*
*	@hash - Hash of node in byte format.
*	@ip - Ip address.
*/
void _nodes::add(unsigned char *hash, string ip)
{
	size_t hs = HASHSIZE;
	string hash_hex;

	if (hash == nullptr || ip.length() < 6
		|| !hash) {
		return;
	}

	this->mute.lock();

	for (auto &p : this->nodes) {
		if (memcmp(hash, p.hash, hs) == 0
			|| ip == p.ip) {
			this->mute.unlock();
			return;
		}
	}

	hash_hex = bin2hex<HASHSIZE>(hash);
	tgndb.remove_node(ip);

	tgndb.new_node(ip, hash_hex);
	this->mute.unlock();
}