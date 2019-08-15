/**
*	library.cpp - Source file of decentralized
*	network TGN.
*
*	@mrrva - 2019
*/
#include "include/library.hpp"
/**
*	Needed namespaces.
*/
using namespace std;
/**
*	the_garlic_network - Initialization of the
*	project.
*/
void the_garlic_network(void)
{
	string pub = tgndb.get_var("PUBLIC_KEY");
	string sec = tgndb.get_var("SECRET_KEY");
	size_t len = HASHSIZE * 2;
	struct tgn_node node;

	if (pub.length() < len || sec.length() < len) {
		tgnencryption.set_keys_hex(pub, sec);
		tgndb.set_var("PUBLIC_KEY", pub);
		tgndb.set_var("SECRET_KEY", sec);
	}
	else {
		tgnencryption.set_keys_hex(pub, sec);
	}

	tgnnodes.db_select();

	if (tgnnodes.size() == 0) {
		cout << "[E]: Node list is empty. Please "
			<< "add list of nodes to database.\n";
		exit(1);
	}

	node = tgnnodes.get_one();
	tgnnetwork.set_node(node);
}
/**
*	tgn_stop - Stop all threads. Call the function
*	when need to stop working.
*/
void tgn_stop(void)
{
	tgnnetwork.stop = true;
	tgnnetwork.wait_threads();
}
/**
*	tgn_restart - Restart threads. Call the function
*	when need to start working after calling tgn_stop.
*/
void tgn_restart(void)
{
	struct tgn_node node;

	if (tgnnodes.size() == 0) {
		cout << "[E]: Node list is empty. Please "
			<< "add list of nodes to database.\n";
		exit(1);
	}

	node = tgnnodes.get_one();
	tgnnetwork.set_node(node);
}
/**
*	tgn_send_msg - Function of sending byte packages
*	to any client of the network.
*
*	@hash - Hash of the network client.
*	@length - Length of package.
*	@msg - Byte package.
*/
size_t tgn_send_msg(const char *hash, size_t length,
	unsigned char *msg)
{
	unsigned char *b_hash, b_msg[TEXTSIZE], *pack;
	const size_t sz = HASHSIZE * 2;
	string hash_string(hash);
	size_t id;

	if (hash_string.length() != sz || !hash
		|| !msg || length > MAXINPUT - 2
		|| length == 0) {
		cout << "[E]: Incorrect args for tgn"
			<< "_send_msg.\n";
		return 0;
	}

	b_hash = hex2bin<sz>(hash_string);
	memset(b_msg, 0x00, TEXTSIZE);
	memcpy(b_msg + 2, msg, length);
	memcpy(b_msg, &length, 2);

	pack = tgnencryption.pack(b_msg, b_hash);
	id = tgnpacks.new_garlic(b_hash, pack, 0, 0);

	return id;
}
/**
*	tgn_is_working - Getting status of the network 
*	threads.
*/
bool tgn_is_working(void)
{
	bool recv = tgnnetwork.th_s.joinable();
	bool send = tgnnetwork.th_s.joinable();

	return recv && send;
}
/**
*	tgn_myhash - Returns user hash (public key) in
*	hex format.
*/
const char *tgn_myhash(void)
{
	unsigned char *hash = tgnencryption.my_hash();
	string hex_hash = bin2hex<HASHSIZE>(hash);

	return hex_hash.c_str();
}
/**
*	tgn_callback - Function setting for calling when
*	new package was received.
*
*	@callback - Pointer to function.
*/
void tgn_callback(void (*callback)(char *, unsigned char *, size_t))
{
	if (!received_messages.set_callback(callback)) {
		cout << "[W]: Incorrect pointer to func.\n";
		return;
	}
}
/**
*	tgn_pack_status - Getting status of sent package.
*
*	@id - Package id.
*/
enum tgn_status tgn_pack_status(size_t id)
{
	struct tgn_garlic pack;

	if (!tgnpacks.find(id, pack)) {
		cout << "[W]: Can't find needed pack.\n";
		return DOESNT_EXISTS;
	}

	return pack.status;
}
/**
*	tgn_resend_package - Trying to resend exists package
*	from the list of garlic packages.
*
*	@id - Package id.
*/
bool tgn_resend_package(size_t id)
{
	if (id == 0) {
		cout << "[W]: Incorrect package id.\n";
		return false;
	}

	return tgnpacks.garlic_resend(id);
}
/**
*	tgn_remove_packages - Removing all packs in the list
*	of garlic packages.
*/
void tgn_remove_packages(void)
{
	tgnpacks.remove_packages();
}