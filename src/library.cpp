//
//
//
//
//	HERE WILL BE FUNCTIONS FOR CALLING FROM YOUR PROJECTS.
//
//
//
//
//


#include "include/encryption.hpp"
#include "include/database.hpp"
#include "include/network.hpp"
#include "include/nodes.hpp"

#include "include/packages.hpp"

using namespace std;




void tgn_send_msg(const char *, const char *);






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
		return;
	}

	tgnencryption.set_keys_hex(pub, sec);
	tgnnodes.db_select();

	if (tgnnodes.size() == 0) {
		cout << "[E]: Node list is empty. Please "
			<< "add list of nodes to database.\n";
		exit(1);
	}

	node = tgnnodes.get_one();
	tgnnetwork.set_node(node);

///////////TESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTEST
	string hash("cedf9127cef70e495836357ecdfb1ad76e0edef48945afca9cf28a749156094b");
	string msg("Hello world!");
	tgn_send_msg(hash.c_str(), msg.c_str());
///////////TESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTEST
}

void tgn_stop(void)
{
	tgnnetwork.stop = true;
	tgnnetwork.wait_threads();
}

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

void tgn_send_msg(const char *hash, const char *msg)
{
	unsigned char *b_hash, b_msg[TEXTSIZE];
	string hash_string(hash);
	const size_t sz = HASHSIZE * 2;
	size_t len = strlen(msg);

	if (hash_string.length() != sz|| !hash
		|| !msg || len + 1 > TEXTSIZE
		|| len == 0) {
		cout << "[E]: Incorrect args for tgn"
			<< "_send_msg.\n";
		return;
	}

	// encryption msg...
	memset(b_msg, 0x00, TEXTSIZE);
	memcpy(b_msg, msg, len + 1);
	// end encryption
	b_hash = hex2bin<sz>(hash_string);
	tgnpacks.new_garlic(b_hash, b_msg, 0);
}

bool tgn_is_working(void)
{
	bool recv = tgnnetwork.th_s.joinable();
	bool send = tgnnetwork.th_s.joinable();

	return recv && send;
}

const char *tgn_myhash(void)
{
	unsigned char *hash = tgnencryption.my_hash();
	string hex_hash = bin2hex<HASHSIZE>(hash);

	return hex_hash.c_str();
}

void tgn_callback(void (*callback)(const char *, const char *))
{
	bool status = received_messages.set_callback(callback);

	if (!status) {
		cout << "[W]: Incorrect pointer to function.\n";
		return;
	}
}


int main()
{
	the_garlic_network();

	while (tgn_is_working()) {
		sleep(2);
	}

cout << "aaa!!!!!!!!\n";
	tgn_stop();
}