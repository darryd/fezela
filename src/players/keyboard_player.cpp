
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
 * class: KeyboardPlayer
 *
 * Get the moves from the keyboard.
 * 
 * Written by Darry Danzig
 *------------------------------------------------------------------------------------------------------*/

#include "../players.h"
#include "str_utl.h"

#define VALID_LENGTH_MOVE_STR 5
#define VALID_LENGTH_POS_STR 2
#define SPACE_LOCATED 2

using namespace std; 

Move KeyboardPlayer::play_turn(const Board &board, Side side) {

  Board copy_board(board);
  Move move;
  string str_move;

  //copy_board.print_castling_info();

  // Get a valid move.
  while (true) {

    // Get valid input
    do {

      cout << endl;
      cout << Utl::str(side) << ":" << endl;
      cout << "Enter move as \"[column][row] [column][row]\"" << endl;
      cout << "eg) \"a2 a4\"" << endl;

      getline (cin, str_move);

      str_move = StrUtl::normalize_str(str_move);

    } while ( !is_valid_str_move(str_move) );

    move = str_to_move(str_move);

    bool is_valid = copy_board.is_valid_move_allow_implied(move, side, true);
#ifndef DEBUG_ALLOW_ILLEGAL_MOVES
    if ( !is_valid )
      cout << "Invalid move." << endl;
    else
#endif
      break;
  }


// TODO: Check if the move requires pawn promotion.

  cout << "\33[2J\33[;H";

  return move;
}

/*--------------------------------------------------------------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------*/
bool KeyboardPlayer::is_valid_str_move(string str_move) {

  string old_pos_str;
  string new_pos_str;

  if ( str_move.length() != VALID_LENGTH_MOVE_STR ) 
    return false;

  if ( str_move[SPACE_LOCATED] != STR_UTL_WHITE_SPACE ) 
    return false;

  old_pos_str = str_move.substr(0, 2);
  new_pos_str = str_move.substr(3, 2);

  return is_valid_str_pos(old_pos_str) && is_valid_str_pos(new_pos_str);
}

/*--------------------------------------------------------------------------------------------------------
 *
 * -------------------------------------------------------------------------------------------------------*/
bool KeyboardPlayer::is_valid_str_pos(string str_pos) {

  if (str_pos.length() != VALID_LENGTH_POS_STR )
    return false;

  if ( str_pos[0] < 'a' && str_pos[0] > 'h' )
    return false;

  if ( str_pos[1] < '1' && str_pos[1] > '8' )
    return false;

  return true;
}

/*--------------------------------------------------------------------------------------------------------
 *
 * Precondition: the string is a valid positional string. Use is_valid_pos_str() method if you need to
 *               check.
 * ------------------------------------------------------------------------------------------------------*/
Position KeyboardPlayer::str_to_position(string str_pos) {

  Position pos;

  pos.x = str_pos[0] - 'a'; 
  pos.y = str_pos[1] - '1';

  return pos;
}

/*--------------------------------------------------------------------------------------------------------
 *
 *
 * Precondition: the string is a valid movement string. Use is_valid_move_str() method if you need to 
 *               check.
 * -------------------------------------------------------------------------------------------------------*/
Move KeyboardPlayer::str_to_move(string str_move) {

  Move move;

  move.old_pos = str_to_position(str_move.substr(0, 2));
  move.new_pos = str_to_position(str_move.substr(3, 2));

  return move;
}

