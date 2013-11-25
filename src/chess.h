
/*

    Copyright 2013 Darry Danzig

    This file is part of Fezela. http://www.fezela.net

    Fezela is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Fezela is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Fezela.  If not, see <http://www.gnu.org/licenses/>.

*/
/*--------------------------------------------------------------------------------------------------------
 * file: chess.h
 *
 * written by Darry Danzig
 * ------------------------------------------------------------------------------------------------------*/

#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <string.h>
#include <pthread.h>

#if 0 
#define DEBUG_ALLOW_ILLEGAL_MOVES // For testing, if you want to be able to move pieces anywhere.
#endif

#define TOTAL_ROWS 8 // Total Number Squares in a row or column (or column).

#define COLOURS_ON

#ifdef COLOURS_ON
// colours for displaying on terminal
// vt100 codes:

#define CODE_RESET "\33[0m"
#define CODE_WHITE_SIDE "\33[37m"  
#define CODE_BLACK_SIDE "\33[32m" 
#define CODE_IN_CHECK "\33[5;31;31m" // Want this to blink.

#else

#define CODE_RESET ""
#define CODE_WHITE_SIDE ""  
#define CODE_BLACK_SIDE "" 
#define CODE_IN_CHECK ""

#endif

enum Side { white, black };
enum Kind { rook, knight, bishop, queen, king, pawn };
enum RookKind { promoted, kings_rook, queens_rook };

/*--------------------------------------------------------------------------------------------------------*/

struct Position;

struct Utl {

  static Side opposite(Side side) {
    return side == white ? black : white;
  }

  static std::string str(Side side) {
    return side == white ? "white" : "black";
  }

  static bool is_on_board(int x, int y) {
    return x >= 0 && x < TOTAL_ROWS && y >= 0 && y < TOTAL_ROWS;
  }

  static int direction(Side side) {
    return side == white ? 1 : -1;
  }

  /* Adds pos to list if pos is on the board. */
  static void push_if_is_on_board(std::vector<Position> &list, Position pos);

  static inline int min (int a, int b) { 
    return a < b ? a : b;
  }

  static inline int max (int a, int b) {
    return a > b ? a : b;
  }


};

/*--------------------------------------------------------------------------------------------------------*/

struct Position {
  int x;
  int y;
  Position(int X=0, int Y=0): x(X), y(Y){}
  Position operator+(const Position &rhs) const {
    return Position(x + rhs.x, y + rhs.y);
  }
  bool operator==(const Position &rhs) const {
    return x == rhs.x && y == rhs.y;
  }
};

/*--------------------------------------------------------------------------------------------------------*/

class Board;

class Piece {

  public:
    Piece (Side side = white, Kind kind = pawn, char ascii = 'p');
    virtual void covers(std::vector<Position> &list_covers, Board &board, Position &pos) = 0;
    virtual void  moves(std::vector<Board> &list_board_moves, Board &board, Position &pos) = 0;

    std::string get_ascii_code ();
    inline Side get_side() { return _side; }
    inline Kind get_kind() { return _kind; }
  protected:
    void covers_in_dir (std::vector<Position> &list_covers, Board &board, Position &pos, int x_dir, int y_dir);
    void  default_moves(std::vector<Board> &list_boards, std::vector<Position> &list_covers, const Board &board, Position &pos);
  private:
    Side _side;
    Kind _kind;
    std::string _ascii_code;
};

/*--------------------------------------------------------------------------------------------------------*/

class Rook : public Piece {
  public: 
    Rook (Side side=white, RookKind rook_kind = promoted):Piece(side, rook, 'r'), _rook_kind(rook_kind){}
    virtual void covers(std::vector<Position> &list_covers, Board &board, Position &pos);
    virtual void  moves(std::vector<Board> &list_board_moves, Board &board, Position &pos);
    inline RookKind get_rook_kind() { return _rook_kind; }
  private:
    RookKind _rook_kind;
};

/*--------------------------------------------------------------------------------------------------------*/

class Knight : public Piece {
  public:
    Knight (Side side=white):Piece(side, knight, 'h'){}
    virtual void covers(std::vector<Position> &list_covers, Board &board, Position &pos);
    virtual void  moves(std::vector<Board> &list_board_moves, Board &board, Position &pos);
};

/*--------------------------------------------------------------------------------------------------------*/

class Bishop : public Piece {

  public:
    Bishop (Side side=white):Piece(side, bishop, 'b'){}
    virtual void covers(std::vector<Position> &list_covers, Board &board, Position &pos);
    virtual void  moves(std::vector<Board> &list_board_moves, Board &board, Position &pos);
};

/*--------------------------------------------------------------------------------------------------------*/

class Queen : public Piece {

  public:
    Queen (Side side=white):Piece(side, queen, 'q'){}
    virtual void covers(std::vector<Position> &list_covers, Board &board, Position &pos);
    virtual void  moves(std::vector<Board> &list_board_moves, Board &board, Position &pos);
};

/*--------------------------------------------------------------------------------------------------------*/

class King : public Piece {

  public:
    King (Side side=white):Piece(side, king, 'k'){}
    virtual void covers(std::vector<Position> &list_covers, Board &board, Position &pos);
    virtual void  moves(std::vector<Board> &list_board_moves, Board &board, Position &pos);
  private:
    void castle(std::vector<Board> &list_board_moves, Board &board);
};

/*--------------------------------------------------------------------------------------------------------*/

class Pawn : public Piece {

  public:
    Pawn (Side side=white);
    virtual void covers(std::vector<Position> &list_covers, Board &board, Position &pos);
    virtual void moves(std::vector<Board> &list_board_moves, Board &board, Position &pos);

  private:
    void forward(std::vector<Position> &move_positions, Board &board, Position &pos);
    void helper_moves(std::vector<Board> &list_board_moves, std::vector<Position> move_positions, Board &board, Position &pos);
    void en_passant(std::vector<Board> &board_moves, Board &board, Position &pos);
};

/*--------------------------------------------------------------------------------------------------------*/

struct Move {

    Position old_pos;
    Position new_pos;

    bool is_castling;
    bool is_en_passant;

    Piece *promotion;

    Position extra_old_pos;
    Position extra_new_pos;

    Move():old_pos(Position(0,0)), new_pos(Position(0,0)), is_castling(false), is_en_passant(false),
    promotion(NULL), extra_old_pos(Position(0,0)), extra_new_pos(Position(0,0)){}
};

/*---------------------------------------------------------------------------------------------------------
 *
 * The CastlingInfo struct is used to determine whether castling is allowed.
 *
 * -------------------------------------------------------------------------------------------------------*/

struct CastlingInfo {

  public: 
    bool did_king_move;
    bool did_k_rook_move;
    bool did_q_rook_move;

    CastlingInfo();

    void clear();

    bool did_rook_move(RookKind rook_kind);

    bool can_castle(Board &board, Side side, RookKind rook_kind);

    static Position get_rook_before_castling_position(Side side, RookKind rook_kind);

    static Position get_rook_after_castling_position(Side side, RookKind rook_kind);

    static Position get_king_before_castling_position(Side side);

    static Position get_king_after_castling_position(Side side, RookKind rook_kind);

  private:

    // This function checks that there are no pieces between the king and rook (specified by rook_kind).
    static bool is_clear_between_king_and_rook(Board &board, Side side, RookKind rook_kind);

    static bool is_safe_to_castle(Board &board, Side side, RookKind rook_kind);
};

/*--------------------------------------------------------------------------------------------------------*/

class Board {

  public:
    Board();
    void clear();
    void set();
    void print();
    void print_check(Side side);

    Move get_move() { return _last_move; }

    std::vector<Board> get_board_moves(Side side);

    // Returns true if side is in check.
    bool is_in_check(Side side);

    // If check or stale mate.
    bool is_mate(Side side);

    inline bool is_check_mate(Side turn) { return is_in_check(turn) && is_mate(turn); }

    inline bool is_stale_mate(Side turn) { return !is_in_check(turn) && is_mate(turn); }

    inline bool is_game_over(Side turn) { return is_mate(turn); }

    void move(Move &move); // Does not check if move is valid to keep things fast. 

    bool operator==(const Board& rhs) const;

    bool is_valid_move(Move &move, Side side);

    // Implied castling is when on the King is moved to indicate castling..
    // If the move is implied castling, the method will fill in the rest of 'move' structure
    bool is_valid_move_allow_implied(Move &move, Side side, bool update_move = false);

    // Do not test for valid range for x and y.
    inline Piece *get_piece(Position &pos) {return _pieces[pos.x][pos.y];}
    inline Piece *get_piece(int x, int y) {return _pieces[x][y];} 
    // An extra if statement will slow things down.
    // Remember, this call could be called within a recurisive 
    // function. So caller has to be careful.

    void set_piece(Piece *piece, int x, int y) { 

      Position pos(x,y);

      return set_piece(piece, pos);
    }

    void set_piece(Piece *piece, const Position &pos) {

      int x = pos.x;
      int y = pos.y;

      // No if statement here because trying to optimize
      _pieces[x][y] = piece;

    }

    inline CastlingInfo * get_castling_info(Side side) { return side == white ? &_w_castling_info : &_b_castling_info; }

    void print_castling_info(Side side);
    void print_castling_info();


  private: 
    Piece * _pieces[TOTAL_ROWS][TOTAL_ROWS];

    int _last_move_count;
    Move _last_move;
    CastlingInfo _w_castling_info;
    CastlingInfo _b_castling_info;


    inline void update_castling_info(Piece *piece);
};

/*--------------------------------------------------------------------------------------------------------*/

class Player {
  public:
    virtual Move play_turn(const Board &board, Side side) = 0;
};

/*--------------------------------------------------------------------------------------------------------*/

struct GameData {

  Board board;
  int seq_no; 
};

/*--------------------------------------------------------------------------------------------------------*/

class Subscriber {
  public:
    Subscriber();
    void notification(GameData game_data, int seq_no);
  protected:
    virtual void do_work(GameData game_data) = 0;
  private:
    void process_waiting_room();
    std::map<int, GameData> _waiting_room;
    int _seq_no;
    pthread_mutex_t _mutex;
};

/*--------------------------------------------------------------------------------------------------------*/

class Game {

  public:
    Game(Player *white_player, Player *black_player);
    void play();
    void subscribe(Subscriber *subscriber);
    void unsubscribe(Subscriber *subscriber); 
    void notify(Board board, int seq_no);
  private:
    Board _board;
    std::map<Side, Player*> _players;
    Side _turn;
    std::vector<Subscriber*> _subscribers;
    friend void*do_notify(void *ptr);
};

/*--------------------------------------------------------------------------------------------------------*/

