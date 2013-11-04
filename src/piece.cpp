
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
 * class: Piece
 *
 * This is the abstract class which chess pieces will inherit from.
 *
 * written by Darry Danzig 
 * ------------------------------------------------------------------------------------------------------*/

#include "chess.h"
#include <cctype>
#include <sstream>

using namespace std;

/*--------------------------------------------------------------------------------------------------------
 * method: constructor
 *
 * Inside of the constructor sets up piece for displaying on the terminal. Uses vt100 codes to display
 * colours to show which side the piece is on. 
 *
 * parameters:
 *
 * side: The side the piece is on. Either white or black.
 * kind: The kind of piece. For example, castle
 *
 *-------------------------------------------------------------------------------------------------------*/
Piece::Piece(Side side, Kind kind, char ascii):_side(side), _kind(kind) {

  string colour;
  ostringstream s;

  // White pieces will be lowercase.
  // Black pieces will be uppercase.
  if (_side == black)
    ascii = toupper(ascii);
  else 
    ascii = tolower(ascii);

  colour = _side == white ? CODE_WHITE_SIDE : CODE_BLACK_SIDE;

  s << colour << ascii << CODE_RESET;

  _ascii_code = s.str();

}

string Piece::get_ascii_code() {
  return _ascii_code;
}

/*--------------------------------------------------------------------------------------------------------
 * method: Piece::covers_in_dir
 * 
 * This method finds all the positions that the piece covers in a direction determined by x_dir and
 * y_dir. This method is used by pieces like bishops or rooks that travel in a straight line.
 *
 * Parameters:
 *
 * list_covers: This function will push the positions covered by the piece into list_covers vector.
 *
 * board: The chess board  
 *
 * pos:   The position where the piece is located
 *
 * x_dir, y_dir: The values here should be -1, 0, 1. Other values won't really be applicable to chess.
 *
 *------------------------------------------------------------------------------------------------------*/

void Piece::covers_in_dir (vector<Position> &list_covers, Board &board, Position &pos, int x_dir, int y_dir) {

  // Rule.. go in direction <x_dir, y_dir> until you encounter end of board or encounter another piece 
  
  Position here = pos; // This is the position of the piece. So this position doesn't get added to list_covers
  
  here.x += x_dir;
  here.y += y_dir;

  while ( Utl::is_on_board(here.x, here.y) ) {
    
    list_covers.push_back(here);

    // Did we encounter another piece?
    if ( board.get_piece(here.x, here.y) )
	break;

    here.x += x_dir;
    here.y += y_dir;

  }
}


/*--------------------------------------------------------------------------------------------------------
 * method Piece::default_moves
 *
 * For most pieces a move is the same as the covers minus any square containing another one of your pieces.
 * 
 * -------------------------------------------------------------------------------------------------------*/
void Piece::default_moves(vector<Board> &list_boards, vector<Position> &list_covers, const Board &board, Position &pos) {

  // Go through the covers. 
  for (vector<Position>::iterator i = list_covers.begin(); i != list_covers.end(); ++i) {

    Board copy_board(board);
    Piece *piece = copy_board.get_piece(*i);

    // You cannot take your own piece. (Or you cannot occupy a square with more than one piece.)
    if ( piece != NULL && piece->get_side() == _side )
      continue;

    Move move(pos, *i);
    
    copy_board.move(move);

    // Check if it's an illegal move. An illegal move is when after you move, you're in check.
    if ( ! copy_board.is_in_check(_side) )
      list_boards.push_back(copy_board);
  }
}
