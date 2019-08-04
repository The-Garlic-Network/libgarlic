/**
*	database.cpp - Source file of decentralized
*	network TGN.
*
*	@mrrva - 2019
*/
#include "include/database.hpp"
/**
*	Needed namespaces.
*/
using namespace std;
/**
*	_database::_database - Constructor of the class.
*	Init structs and vars.
*/
_database::_database(void)
{
	string path = "./tgn.db";
	db = nullptr;

	if (sqlite3_open(path.c_str(), &db) != SQLITE_OK) {
		cout << "[E] _database::_database.\n";
		exit(1);
	}

	this->create_tables();
}
/**
*	_database::~_database - Destructor of the class.
*	Destroying of pointers.
*/
_database::~_database(void)
{
	if (this->db != nullptr)
		sqlite3_close(this->db);
}
/**
*	_database::set_var - Adding new settings to the
*	database.
*
*	@name - Name of the option.
*	@value - Value of the option.
*/
void _database::set_var(string name, string value)
{
	sqlite3_stmt *rs = nullptr;
	string q;

	if (name.length() < 2 || value.length() < 1) {
		cout << "[E] _database::set_var(args..).\n";
		return;
	}

	this->mute.lock();

	q = "INSERT INTO settings VALUES('" + name + "', '" +
		value + "')";
	sqlite3_prepare_v2(this->db, q.c_str(), -1, &rs, NULL);

	if (sqlite3_step(rs) != SQLITE_DONE)
		cout << "[E] _database::set_var.\n";

	this->mute.unlock();

	if (rs != nullptr)
		sqlite3_finalize(rs);
}
/**
*	_database::get_var - Selection needed data and
*	settings from the database.
*
*	@name - Name of the option.
*/
string _database::get_var(string name)
{
	sqlite3_stmt *rs = nullptr;
	typedef unsigned char uc;
	unsigned char *f_data;
	string q, r_data;

	if (name.length() < 2) {
		cout << "[E] _database::get_var(arg..).\n";
		return "";
	}

	this->mute.lock();

	q = "SELECT `value` FROM `settings` WHERE `name`='" +
		name + "'";
	sqlite3_prepare_v2(this->db, q.c_str(), -1, &rs, NULL);

	if (sqlite3_step(rs) != SQLITE_ROW) {
		cout << "[E] _database::get_var.\n";

		if (rs != nullptr)
			sqlite3_finalize(rs);
		this->mute.unlock();
		
		return "";
	}

	this->mute.unlock();

	f_data = const_cast<uc *>(sqlite3_column_text(rs, 0));
	r_data = string(reinterpret_cast<char *>(f_data));

	if (rs != nullptr)
		sqlite3_finalize(rs);

	return r_data;
}
/**
*	_database::remove_var - Removing vars from the
*	database.
*
*	@name - Name of the option.
*/
void _database::remove_var(string name)
{
	sqlite3_stmt *rs = nullptr;
	string q;

	if (name.length() < 2) {
		cout << "[E] _database::remove_var.\n";
		return;
	}

	this->mute.lock();

	q = "DELETE FROM `settings` WHERE `name`='" +
		name + "'";

	sqlite3_prepare_v2(this->db, q.c_str(), -1, &rs, NULL);
	sqlite3_step(rs);

	this->mute.unlock();

	if (rs != nullptr)
		sqlite3_finalize(rs);
}
/**
*	_database::remove_node - Removeing data of nodes
*	from the database.
*
*	@ip - Ip address of node.
*/
void _database::remove_node(string ip)
{
	sqlite3_stmt *rs = nullptr;
	string q;

	if (ip.length() < 5) {
		cout << "[E] _database::remove_node.\n";
		return;
	}

	this->mute.lock();

	q = "DELETE FROM `nodes` WHERE `ip`='" + ip + "'";
	sqlite3_prepare_v2(this->db, q.c_str(), -1, &rs, NULL);
	sqlite3_step(rs);

	this->mute.unlock();

	if (rs != nullptr)
		sqlite3_finalize(rs);
}
/**
*	_database::new_node - Adding new node to the
*	database.
*
*	@ip - Ip address.
*	@hash - Node's hash.
*/
void _database::new_node(string ip, string hash)
{
	sqlite3_stmt *rs = nullptr;
	stringstream ss;

	if (ip.length() < 5) {
		cout << "[E] _database::new_node.\n";
		return;
	}

	this->mute.lock();

	ss << "INSERT INTO `nodes` VALUES (NULL, '" << ip
		<< "', '" << hash << "');";
	sqlite3_prepare_v2(this->db, ss.str().c_str(), -1,
		&rs, NULL);
	sqlite3_step(rs);

	this->mute.unlock();

	if (rs != nullptr)
		sqlite3_finalize(rs);
}
/**
*	_database::select_nodes - Selecting all nodes
*	form the database.
*/
map<string, string> _database::select_node(void)
{
	string q = "SELECT * FROM `nodes`", f, s;
	sqlite3_stmt *rs = nullptr;
	map<string, string> list;

	auto get_str = [](sqlite3_stmt *r, int n) {
		typedef unsigned char uc;
		unsigned char *data;
		char *buffer;

		data = const_cast<uc *>(sqlite3_column_text(r, n));
		buffer = reinterpret_cast<char *>(data);

		return string(buffer);
	};

	sqlite3_prepare_v2(this->db, q.c_str(), -1, &rs, NULL);

	while (sqlite3_step(rs) == SQLITE_ROW) {
		f = get_str(rs, 1);
		s = get_str(rs, 2);
		list.insert(pair<string, string>(f, s));
	}

	if (rs != nullptr)
		sqlite3_finalize(rs);

	return list;
}
/**
*	_database::create_tables - Creating tables of the
*	network's database.
*/
void _database::create_tables(void)
{
	vector<string> query = {
		"CREATE TABLE IF NOT EXISTS    " \
		"`nodes` (`id` INTEGER PRIMARY " \
		"KEY AUTOINCREMENT, `ip` text  " \
		"NOT NULL,`hash` text NOT NULL);",

		"CREATE TABLE IF NOT EXISTS    " \
		"`settings` (`name` text NOT   " \
		"NULL, `value` text NOT NULL);"
	};
	sqlite3_stmt *rs = nullptr;

	for (auto &str : query) {
		sqlite3_prepare_v2(this->db, str.c_str(), -1,
			&rs, NULL);

		if (sqlite3_step(rs) != SQLITE_DONE) {
			cout << "[E] _database::create_tables.\n";
			exit(1);
		}
	}

	if (rs != nullptr)
		sqlite3_finalize(rs);
}