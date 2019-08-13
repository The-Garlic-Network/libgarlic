

CFLAGS	= -pthread -lsqlite3 -lsodium
NAME	= libgarlic.so
CC		= g++

all:
	$(CC) -Wall -fPIC -c src/*.cpp -std=c++17
	$(CC) -shared -Wl,-soname,$(NAME).1 -o $(NAME) *.o $(CFLAGS)
	cp $(NAME) $(NAME).1
	cp src/garlic.h garlic.h

clear:
	rm -f *.o garlic.h

example:
	gcc -Wall -std=c11 test.c -Wl,--rpath="/home/mrrva/Рабочий стол/libgarlic" \
	-L. -I. $(CFLAGS)
