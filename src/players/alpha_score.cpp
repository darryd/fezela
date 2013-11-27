
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
#include "../chess.h"
#include "../score.h"

using namespace std; 

AlphaScore::AlphaScore() {

  values[king] = 0;
  values[queen] = 9;
  values[rook] = 5;
  values[bishop] = 3;
  values[knight] = 3;
  values[pawn] = 1;

}

int AlphaScore::get_score(const Board &board, Side side) {

  int score = 0;
  Board copy_board(board);

  for (int x=0; x<8; x++)
    for (int y=0; y<8; y++) {

      Piece *piece = copy_board.get_piece(x, y);

      if (piece == NULL)
	continue;

      score += values[piece->get_kind()] * (side == piece->get_side() ? 1 : -1);
    }
  return score;
}
