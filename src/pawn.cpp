
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
 * class Pawn
 *
 * written by Darry Danzig
 * ------------------------------------------------------------------------------------------------------*/

#include "chess.h"
using namespace std;

vector<Piece*> get_promotional_pieces(Side side);

Pawn::Pawn(Side side):Piece(side) {}

void Pawn::covers(vector<Position> &list_covers, Board &board, Position &pos) {

  int dir = Utl::direction(get_side());

  Position left  = pos + Position(-1, dir);
  Position right = pos + Position( 1, dir);


  Utl::push_if_is_on_board(list_covers, left);
  Utl::push_if_is_on_board(list_covers, right);


}

void Pawn::moves(std::vector<Board> &list_board_moves, Board &board, Position &pos) {

  vector<Position> move_positions;

  Piece *piece_left = NULL;
  Piece *piece_right = NULL;

  int dir = Utl::direction(get_side());

  Position left  = pos + Position(-1, dir);
  Position right = pos + Position( 1, dir);

  if ( Utl::is_on_board(left.x, left.y) ) {

    piece_left = board.get_piece(left);

    if ( piece_left != NULL && piece_left->get_side() != get_side() ) {
      Utl::push_if_is_on_board(move_positions, left);
    }

  }

  if ( Utl::is_on_board(right.x, right.y) ) {
    piece_right = board.get_piece(right);

    if ( piece_right != NULL && piece_right->get_side() != get_side() ) {
	Utl::push_if_is_on_board(move_positions, right);
    }

  }

  forward(move_positions, board, pos);

//  default_moves(list_board_moves, move_positions, board, pos);
   helper_moves(list_board_moves, move_positions, board, pos);
}

/*--------------------------------------------------------------------------------------------------------
 * method: Pawn::forward
 *
 * This method just looks to see if the pawn can move forward. If it can it adds the forward moves to the
 * vector move_positions. This is helper method for the moves method.
 *
 * TODO: en passent
 *-------------------------------------------------------------------------------------------------------*/
void Pawn::forward(vector<Position> &move_positions, Board &board, Position &pos) {

  int dir = Utl::direction(get_side());    // The direction the pawn travels in.
  int starting_y = get_side() == white ? 1 : 6; // In this position a pawn may move ahead by two squares.

  Position here = pos + Position(0, dir);

  if ( !Utl::is_on_board(here.x, here.y) ) 
    return; // Pawn is at end of board. Cannot advance.

  if ( board.get_piece(here.x, here.y) != NULL )
    return; // Pawn is blocked by a piece. Cannot advance.

  move_positions.push_back(here);

  // Is the pawn at the starting position? Pawns in the starting position can advance by two squares
  // if they are not blocked by another piece.
  if ( pos.y != starting_y )
    return;

  here = here + Position(0, dir);

  if ( board.get_piece(here.x, here.y) != NULL )
    return; // Pawn is blocked by a piece. Cannot advance.

  move_positions.push_back(here);
}

/*------------------------------------------------------------------------------------------------------
 * method helper_moves
 *
 * checks for promotion and makes sure a move is legal (i.e one's king is left in check).
 *
 * this method generates moves from the list of move_positions.
 *
 * -----------------------------------------------------------------------------------------------------*/
void Pawn::helper_moves(std::vector<Board> &list_board_moves, vector<Position> move_positions, Board &board, Position &pos) {

  int last_row = get_side() == white ? 7 : 0;

  for (vector<Position>::iterator i = move_positions.begin(); i != move_positions.end(); ++i) {

    Position new_pos = *i;
    Move move;
    Board copy_board(board);

    move.old_pos = pos;
    move.new_pos = new_pos;
    
    copy_board.move(move);

    if ( copy_board.is_in_check(get_side()) )
      continue; // Illegal move!

    if ( new_pos.y != last_row )
      list_board_moves.push_back(copy_board);
    else {

      // Pawn promotion! 

      vector<Piece*> promotional_pieces = get_promotional_pieces(get_side());
      
      for (vector<Piece*>::iterator i = promotional_pieces.begin(); i != promotional_pieces.end(); ++i) {

	Board copy_board(board);
	Move promote_move(move);

	promote_move.promotion = *i;
	copy_board.move(promote_move);

	list_board_moves.push_back(copy_board);
      }
    }
  }
}
