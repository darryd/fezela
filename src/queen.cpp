
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
 * class Queen
 *
 * written by Darry Danzig
 * ------------------------------------------------------------------------------------------------------*/

#include "chess.h"
using namespace std;

void Queen::covers(vector<Position> &list_covers, Board &board, Position &pos) {
  

  // Queen = Bishop + Rook

  // From Bishop:
  covers_in_dir(list_covers, board, pos, 1, 1);
  covers_in_dir(list_covers, board, pos, 1, -1);
  covers_in_dir(list_covers, board, pos, -1, -1);
  covers_in_dir(list_covers, board, pos, -1, 1);

  
  // From Rook:
  covers_in_dir(list_covers, board, pos, 0, 1);
  covers_in_dir(list_covers, board, pos, 1, 0);
  covers_in_dir(list_covers, board, pos, 0, -1);
  covers_in_dir(list_covers, board, pos, -1, 0);

}
void Queen::moves(std::vector<Board> &list_board_moves, Board &board, Position &pos) {

  vector<Position> list_covers;

  covers(list_covers, board, pos);
  default_moves(list_board_moves, list_covers, board, pos);

}
