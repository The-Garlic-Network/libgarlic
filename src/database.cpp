/**
*	database.cpp - Модуль отвечающий за работу
*	с базой данных.
*
*	@mrrva - 2019
*/
#include "include/database.hpp"
/**
*	Используемые пространства имен и объекты.
*/
using namespace std;
/**
*	_database::_database - Конструктор класса _database.
*	Заполняет переменные и указатели.
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
*	_database::get_nodes - Извлечение списка всех нод
*	из базы данных.
*/
map<string, string> _database::get_nodes(void)
{
	string q = "SELECT * FROM `nodes`";
	map<string, string> list;
	sqlite3_stmt *rs = nullptr;
	char *d1, *d2;

	sqlite3_prepare_v2(this->db, q.c_str(), -1, &rs, NULL);

	while (sqlite3_step(rs) == SQLITE_ROW) {
		d1 = const_cast<char *>(sqlite3_column_text(rs, 1));
		d2 = const_cast<char *>(sqlite3_column_text(rs, 2));

		list.insert(string(tmp1), string(tmp2))
	}

	if (rs && rs != nullptr)
		sqlite3_finalize(rs);

	return list;
}
/**
*	_database::remove_node - Удаление ноды из базы данных.
*
*	@ip - Ip адрес ноды.
*/
void _database::remove_node(string ip)
{
	sqlite3_stmt *rs = nullptr;
	string q;

	if (ip.length() < 5) {
		cout << "[E] _database::remove_node.\n";
		return;
	}

	q = "DELETE FROM `nodes` WHERE `ip`='" + ip + "'";
	sqlite3_prepare_v2(this->db, q.c_str(), -1, &rs, NULL);
	sqlite3_step(rs);

	if (rs != nullptr)
		sqlite3_finalize(rs);
}
/**
*	_database::new_node - Добавление новой ноды в базу
*	данных.
*
*	@ip - Ip адрес ноды.
*	@hash - Hash ноды.
*/
void _database::new_node(string ip, string hash)
{
	sqlite3_stmt *rs = nullptr;
	string q;

	if (ip.length() < 6 || hash.length() != HASHSIZE) {
		cout << "[E] _database::new_node.\n";
		return;
	}

	q = "INSERT INTO `nodes` VALUES (NULL, '" + ip +
		"', '" + hash + "');";

	sqlite3_prepare_v2(this->db, q.c_str(), -1, &rs, NULL);
	sqlite3_step(rs);

	if (rs && rs != nullptr)
		sqlite3_finalize(rs);
}
/**
*	_database::get_var - Извлечения данных настроек из
*	базы данных.
*
*	@name - Имя параметра.
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


	q = "SELECT `value` FROM `settgngs` WHERE `name`='" +
		name + "'";
	sqlite3_prepare_v2(this->db, q.c_str(), -1, &rs, NULL);

	if (sqlite3_step(rs) != SQLITE_ROW) {
		cout << "[E] _database::get_var.\n";

		if (rs != nullptr)
			sqlite3_finalize(rs);
		
		return "";
	}

	f_data = const_cast<uc *>(sqlite3_column_text(rs, 0));
	r_data = string(reinterpret_cast<char *>(f_data));

	if (rs != nullptr)
		sqlite3_finalize(rs);

	return r_data;
}
/**
*	_database::remove_var - Удаление данных настройки
*	из базы данных.
*
*	@name - Имя параметра.
*/
void _database::remove_var(string name)
{
	sqlite3_stmt *rs = nullptr;
	string q;

	if (name.length() < 2) {
		cout << "[E] _database::remove_var.\n";
		return;
	}

	q = "DELETE FROM `settgngs` WHERE `name`='" +
		name + "'";

	sqlite3_prepare_v2(this->db, q.c_str(), -1, &rs, NULL);
	sqlite3_step(rs);

	if (rs != nullptr)
		sqlite3_finalize(rs);
}
/**
*	_database::create_tables - Функция создания таблиц
*	базы данных сети.
*/
void _database::create_tables(void)
{
	vector<string> query = {
		"CREATE TABLE IF NOT EXISTS    " \
		"`nodes` (`id` INTEGER PRIMARY " \
		"KEY AUTOINCREMENT, `ip` text  " \
		"NOT NULL,`hash` text NOT NULL);",

		"CREATE TABLE IF NOT EXISTS    " \
		"`settgngs` (`name` text NOT   " \
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