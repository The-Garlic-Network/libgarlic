FLAGS = -lsodium -pthread -lsqlite3 -lgarlic -std=c11 -Wall -L. -I.

example_1:
	gcc -Wall 1_receiver.c -o receiver -Wl,--rpath="./" $(FLAGS)
	gcc -Wall 1_sender.c -o sender -Wl,--rpath="./" $(FLAGS)
