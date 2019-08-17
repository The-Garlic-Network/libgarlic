
EX_FLAGS = -lsodium -pthread -lsqlite3 -lgarlic -std=c11 -Wall -L"./example" -I"./example"
CFLAGS	= -pthread -lsqlite3 -lsodium
NAME	= libgarlic
EX_CC	= gcc
CC		= g++
OS 		= $(shell uname)

all: library ex clear

library:
	$(CC) -Wall -fPIC -c src/*.cpp -std=c++17

ifeq ($(OS),Darwin)
	$(CC) -dynamiclib -undefined suppress -flat_namespace *.o -o $(NAME).dylib
else	
	$(CC) -shared -Wl,-soname,$(NAME).so.1 -o $(NAME).so *.o $(CFLAGS)
	cp $(NAME).so $(NAME).so.1
endif

	cp src/garlic.h garlic.h

ex:
	cp garlic.h example/garlic.h

ifeq ($(OS),Darwin)
	cp $(NAME).dylib example/$(NAME).dylib
	$(EX_CC) -Wall example/1_receiver.c -o example/receiver $(EX_FLAGS)
	$(EX_CC) -Wall example/1_sender.c -o example/sender $(EX_FLAGS)
else
	cp $(NAME).so example/$(NAME).so
	cp $(NAME).so.1 example/$(NAME).so.1
	$(EX_CC) -Wall example/1_receiver.c -o example/receiver -Wl,--rpath="./example" $(EX_FLAGS)
	$(EX_CC) -Wall example/1_sender.c -o example/sender -Wl,--rpath="./example" $(EX_FLAGS)
endif


clear:
	rm -f *.o