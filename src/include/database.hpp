
#ifndef TGN_DATABASE
#define TGN_DATABASE

#include "tgn.hpp"

class _database {
	private :
		sqlite3 *db;

		void create_tables(void);

	public :
		std::map<std::string, std::string> get_nodes(void);
		void new_node(std::string, std::string);
		std::string get_var(std::string);
		void remove_node(std::string);
		void remove_var(std::string);
		_database(void);

};

inline _databse tindb;

#endif