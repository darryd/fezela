VPATH = players/
CFLAGS = -Wall -g
LIBS = -lpthread
MODULES = \
      build/main.o \
      build/rook.o \
      build/knight.o \
      build/bishop.o \
      build/queen.o \
      build/king.o \
      build/pawn.o \
      build/piece.o \
      build/board.o \
      build/castling_info.o \
      build/counting_and_checking_ai.o \
      build/game.o \
      build/keyboard_player.o \
      build/random_ai.o \
      build/simply_counting_ai.o \
      build/str_utl.o \
      build/subscriber.o \
      build/utl.o 

all: build fezela

build:
	mkdir -p build

build/%.o: %.cpp chess.h
	$(CXX) $(CFLAGS) $(LIBS) -c $< -o $@

fezela: $(MODULES)
	g++ $(CFLAGS) $(MODULES) $(LIBS) -o fezela

clean:
	rm -rf build
	rm -rf fezela

