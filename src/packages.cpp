/**
*	packages.cpp - Source file of decentralized
*	network TGN.
*
*	@mrrva - 2019
*/
#include "include/packages.hpp"
/**
*	Needed namespaces.
*/
using namespace std;
/**
*	_packages::add_bytes - Add new package for processing
*	in byte format.
*
*	@bytes - Byte array of package.
*/
void _packages::add_bytes(unsigned char *bytes)
{
	tgnmsg message;

	if (!bytes || bytes == nullptr) {
		return;
	}

	message = tgnmsg(bytes);
	this->add_msg(message);
}
/**
*	_packages::add_msg - Add new package for processing
*	in tgnmsg format.
*
*	@message - Package.
*/
void _packages::add_msg(tgnmsg message)
{
	enum tgn_htype type = message.header_type();
	struct tgn_pack pack;
	unsigned char *bytes;
	size_t len = 0;

	if (type == INDEFINITE_MESSAGE
		|| type == REQUEST_DOS) {
		return;
	}

	pack.ping =system_clock::now();
	bytes = message.to_bytes(len);
	memcpy(pack.buff, bytes, len);
	pack.length = len;

	this->mute.lock();
	this->packs.push_back(pack);
	this->mute.unlock();

	delete[] bytes;
}
/**
*	_packages::resp_msg - Processing response of first
*	request in the list.
*
*	@message - Package.
*/
void _packages::resp_msg(tgnmsg &message)
{
	enum tgn_htype type = message.header_type();

	if (type == INDEFINITE_MESSAGE
		|| type == RESPONSE_NODES) {
		return;
	}

	if (type == RESPONSE_GARLIC) {
		this->garlic_status(message);
		return;
	}

	if (type == RESPONSE_NODES) {
		this->nodes_response(message);
		return;
	}

	this->remove_one(type);
}
/**
*	_packages::resp_msg - Removing done task from
*	the list.
*
*	@type - Type of package.
*/
bool _packages::remove_one(enum tgn_htype type)
{
	vector<struct tgn_pack>::iterator it;
	bool status = false;

	this->mute.lock();
	it = this->packs.begin();

	for (; it != this->packs.end(); it++) {
		if ((*it).buff[0] + 1 == type) {
			this->packs.erase(it);
			status = true;
			break;
		}
	}

	this->mute.unlock();
	return status;
}
/**
*	_packages::last_id - Returns last id of garlic
*	package.
*/
size_t _packages::last_id(void)
{
	size_t id;

	this->mute.lock();

	if (this->msgs.size() == 0) {
		this->mute.unlock();
		return 0;
	}

	id = (*this->msgs.end()).id;
	this->mute.unlock();
	
	return id;
}
/**
*	_packages::new_garlic - New garlic message.
*
*	@hash - To whom (hash in byte format).
*	@text - Message with TEXTSIZE size.
*	@attempt - Attempt of sending.
*	@rid - If package already exists - put it here.
*/
size_t _packages::new_garlic(unsigned char *hash,
	unsigned char *text, size_t attempt, size_t rid)
{
	size_t id = this->last_id();
	struct tgn_garlic one;

	if (hash == nullptr || !hash || text == nullptr
		|| !text) {
		cout << "[W]: Incorrect args for new "
			<< "garlic message.\n";
		return 0;
	}

	one.id = (rid == 0) ? ++id : rid;
	one.ping = system_clock::now();
	one.status = NEW_PACKAGE;
	one.attempts = attempt;

	memset(one.text, 0x00, TEXTSIZE);
	memcpy(one.to, hash, HASHSIZE);
	memcpy(one.text, text, TEXTSIZE);

	this->mute.lock();
	this->msgs.push_back(one);
	this->mute.unlock();

	return one.id;
}
/**
*	_packages::first_ping - A time of creating first 
*	package in the list.
*/
time_point<system_clock> _packages::first_ping(void)
{
	auto ping = system_clock::now();

	this->mute.lock();

	if (this->packs.size() == 0) {
		this->mute.unlock();
		return ping;
	}

	ping = (*this->packs.begin()).ping;
	this->mute.unlock();

	return ping;
}
/**
*	_packages::first_ping - Set new time for all
*	packages in the list.
*/
void _packages::ping_packages(void)
{
	this->mute.lock();

	for (auto &p : this->packs) {
		p.ping = system_clock::now();
	}

	for (auto &e : this->msgs) {
		e.ping = system_clock::now();
	}

	this->mute.unlock();
}
/**
*	_packages::first_ping - Getting current package
*	for sending.
*/
struct tgn_pack _packages::get_one(void)
{
	auto time = system_clock::now();
	struct tgn_garlic msg;
	struct tgn_pack pack;

	this->mute.lock();

	if (this->garlic_size() == 0) {
		pack = *this->packs.begin();
		this->mute.unlock();
		return pack;
	}

	for (auto &p : this->msgs) {
		if (p.status == NEW_PACKAGE) {
			p.status = EMPTY_STATUS;
			msg = p;
			break;
		}

		if (p.status == EMPTY_STATUS
			&& time - p.ping >= 15s) {
			p.ping = system_clock::now();
			msg = p;
			break;
		}

		if (p.status == REQUEST_FIND
			&& time - p.ping >= 15s) {
			p.ping = system_clock::now();
			p.attempts++;
			msg = p;
			break;
		}
	}

	pack = this->garlic_to_pack(msg);
	this->mute.unlock();

	return pack;
}
/**
*	_packages::size - Full size of all packages in lists.
*/
size_t _packages::size(void)
{
	this->mute.lock();
	size_t size = this->packs.size() + this->garlic_size();
	this->mute.unlock();

	return size;
}
/**
*	_packages::system_request - Creating a new system
*	package.
*/
void _packages::system_request(enum tgn_htype type)
{
	tgnmsg message;

	message.create(type);
	this->add_msg(message);
}
/**
*	_packages::garlic_size - Getting size of elements
*	in the garlic's list.
*/
size_t _packages::garlic_size(void)
{
	auto time = system_clock::now();
	size_t size = 0;

	for (auto &p : this->msgs) {
		if (p.status == NEW_PACKAGE) {
			size++;
			continue;
		}

		if (p.status == EMPTY_STATUS
			&& time - p.ping >= 15s) {
			size++;
			continue;
		}

		if (p.status == REQUEST_FIND
			&& p.attempts > 7) {
			p.status = ERROR_TARGET;
			continue;
		}

		if (p.status == REQUEST_FIND
			&& time - p.ping >= 15s) {
			size++;
			continue;
		}
	}

	return size;
}
/**
*	_packages::nodes_response - Process response for request
*	of getting new nodes.
*
*	@message - Package of response.
*/
void _packages::nodes_response(tgnmsg &message)
{
	unsigned char hash[HASHSIZE], b[4], *info;
	size_t parts = INFOSIZE / (HASHSIZE + 4);
	string ip;

	if (!this->remove_one(RESPONSE_NODES)) {
		return;
	}

	info = message.get_info();

	for (size_t i = 0; i < parts; i ++) {
		memcpy(b, info + HASHSIZE, 4);
		memcpy(hash, info, HASHSIZE);

		ip = ipfrombytes(b);

		if (bytes_sum<HASHSIZE>(hash) == 0
			|| ip == "0.0.0.0") {
			delete[] info;
			break;
		}

		tgnnodes.add(hash, ip);
		info += HASHSIZE + 4;
	}

	delete[] info;
}
/**
*	_packages::garlic_to_pack - Convert tgn_garlic struct
*	to tgn_pack struct.
*
*	@one - tgn_garlic struct.
*/
struct tgn_pack _packages::garlic_to_pack(
	struct tgn_garlic &one)
{
	unsigned char *bytes;
	struct tgn_pack pack;
	tgnmsg msg(nullptr);
	size_t len;

	msg.create(REQUEST_GARLIC, one.text, one.to);
	bytes = msg.to_bytes(len);

	memcpy(pack.buff, bytes, len);
	pack.ping = system_clock::now();
	pack.length = len;

	delete[] bytes;
	return pack;
}
/**
*	_packages::garlic_status - Process garlic request
*	from nodes and clients.
*
*	@message - Package.
*/
void _packages::garlic_status(tgnmsg &message)
{
	struct tgn_garlic one = message.info_garlic();
	vector<struct tgn_garlic>::iterator it;
	size_t hs = HASHSIZE;

	this->mute.lock();
	it = this->msgs.begin();

	for (; it != this->msgs.end(); it++) {
		if (memcmp(one.to, (*it).to, hs) != 0)
			continue;

		(*it).ping = system_clock::now();
		(*it).status = one.status;
		break;
	}

	this->mute.unlock();
}
/**
*	_packages::garlic_resend - Trying to resend exists
*	package.
*
*	@id - Id of package.
*/
bool _packages::garlic_resend(size_t id)
{
	vector<struct tgn_garlic>::iterator it;
	unsigned char *txt;
	bool done = false;
	size_t atts;

	this->mute.lock();
	it = this->msgs.begin();

	for (; it != this->msgs.end(); it++) {
		if ((*it).id != id) {
			continue;
		}

		atts = (*it).attempts + 1;
		txt = (*it).text;
		done = true;

		this->new_garlic((*it).to, txt, atts,
						(*it).id);
		this->msgs.erase(it);
		break;
	}

	this->mute.unlock();
	return done;
}
/**
*	_packages::remove_packages - Removing all packs
*	in the list of garlic packages.
*/
void _packages::remove_packages(void)
{
	this->mute.lock();

	if (this->msgs.size() == 0) {
		this->mute.unlock();
		return;
	}

	for (size_t i = 0; i < this->msgs.size(); i++) {
		this->msgs.erase(this->msgs.begin() + i);
	}

	this->mute.unlock();
}
/**
*	_packages::find - Find exists package in the list.
*
*	@id - Id of package.
*	@buff - Buffer for returning data. 
*/
bool _packages::find(size_t id, struct tgn_garlic &buff)
{
	bool status = false;

	this->mute.lock();

	for (auto &p : this->msgs) {
		if (p.id != id)
			continue;

		status = true;
		buff = p;
		break;
	}

	this->mute.unlock();
	return status;
}