
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
 * class Knight
 *
 * written by Darry Danzig
 * ------------------------------------------------------------------------------------------------------*/

#include "chess.h"
using namespace std;

void Knight::covers(vector<Position> &list_covers, Board &board, Position &pos) {

  // All possible moves a knight can make. 8 in total,
  // When the row is 2, column can by -1 or 1
  // and when the row is 1, the column can be -2 or 2

  Utl::push_if_is_on_board(list_covers, pos + Position(2, 1));
  Utl::push_if_is_on_board(list_covers, pos + Position(2,-1));

  Utl::push_if_is_on_board(list_covers, pos + Position(1, 2));
  Utl::push_if_is_on_board(list_covers, pos + Position(1,-2));

  Utl::push_if_is_on_board(list_covers, pos + Position(-1, 2));
  Utl::push_if_is_on_board(list_covers, pos + Position(-1,-2));

  Utl::push_if_is_on_board(list_covers, pos + Position(-2, 1));
  Utl::push_if_is_on_board(list_covers, pos + Position(-2,-1));

}

void  Knight::moves(std::vector<Board> &list_board_moves, Board &board, Position &pos) {

  vector<Position> list_covers;

  covers(list_covers, board, pos);
  default_moves(list_board_moves, list_covers, board, pos);

}
