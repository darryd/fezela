VPATH = players/

CC = g++ 

DEBUG =

CFLAGS = $(DEBUG) -c -Wall -g -pthread

PLAYER_O_FILES = random_ai.o simply_counting_ai.o counting_and_checking_ai.o \
		 keyboard_player.o str_utl.o

all: fezela

fezela: castling_info.o game.o global_pieces.h players.h chess.h main.o piece.o \
  rook.o knight.o bishop.o queen.o king.o pawn.o board.o utl.o $(PLAYER_O_FILES) 
	$(CC) $(DEBUG) -pthread -g -Wall castling_info.o game.o main.o piece.o rook.o knight.o \
	  bishop.o queen.o king.o pawn.o board.o utl.o $(PLAYER_O_FILES) -o fezela

castling_info: chess.h castling_info.o
	$(CC) $(CFLAGS) castling_info.cpp

game: chess.h game.cpp
	$(CC) $(CFLAGS) game.cpp

counting_and_checking_ai: chess.h players.h counting_and_checking_ai.o
	$(CC) $(CFLAGS) counting_and_checking_ai.cpp

random_ai: chess.h players.h random_ai.o
	$(CC) $(CFLAGS) random_ai.cpp

simply_counting_ai: chess.h players.h simply_counting_ai.o
	$(CC) $(CFLAGS) simply_counting_ai.cpp

str_utl: players/str_utl.h str_utl.o
	$(CC) $(CFLAGS) str_utl.cpp

keyboard_player: chess.h players.h keyboard_player.o
	$(CC) $(CFLAGS) keyboard_player.cpp

main: chess.h main.o
	$(CC) $(CFLAGS) main.cpp

piece: chess.h piece.o
	$(CC) $(CFLAGS) piece.cpp

rook: chess.h rook.o
	$(CC) $(CFLAGS) rook.cpp

knight: chess.h knight.o
	$(CC) $(CFLAGS) knight.cpp

bishop: chess.h bishop.o
	$(CC) $(CFLAGS) bishop.cpp

queen: chess.h queen.o
	$(CC) $(CFLAGS) queen.cpp

king: chess.h king.o
	$(CC) $(CFLAGS) king.cpp

pawn: chess.h pawn.o
	$(CC) $(CFLAGS) pawn.cpp

utl: chess.h utl.o
	$(CC) $(CFLAGS) utl.cpp
	
board: chess.h board.o
	$(CC) $(CFLAGS) board.cpp

clean:
	rm *.o fezela
