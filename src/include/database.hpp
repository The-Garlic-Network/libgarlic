/**
*	database.hpp - Header file of decentralized
*	network TGN. Here is published class of database.
*
*	@mrrva - 2019
*/
#ifndef TGN_DATABASE
#define TGN_DATABASE
/**
*	Included header files.
*/
#include "tgn.hpp"
/**
*	Main class of the module.
*/
class _database {
	private :
		std::mutex mute;
		sqlite3 *db;

		void create_tables(void);

	public :
		std::map<std::string, std::string> select_node(void);
		void new_node(std::string, std::string);
		void set_var(std::string, std::string);
		std::string get_var(std::string);
		void remove_node(std::string);
		void remove_var(std::string);
		~_database(void);
		_database(void);
};
/**
*	Object of module controlling.
*/
inline _database tgndb;

#endif