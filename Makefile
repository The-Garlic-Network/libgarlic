all:
	g++ -Wall -g3 -std=c++17 src/message.cpp  src/network.cpp src/packages.cpp\
		src/received.cpp src/database.cpp src/encryption.cpp src/library.cpp\
		src/nodes.cpp  -pthread -lsqlite3 -lsodium
