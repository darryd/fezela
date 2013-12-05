
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
#include <climits>

#define THEM_CHECK_MATE_SCORE INT_MAX
#define US_CHECK_MATE_SCORE INT_MIN
#define US_STALE_MATE_SCORE -1000000
#define THEM_STALE_MATE_SCORE -US_STALE_MATE_SCORE

#define POINTS_FOR_COVERING 10

using namespace std; 

AlphaScore::AlphaScore() {

  values[king] = 0;
  values[queen] = 9;
  values[rook] = 5;
  values[bishop] = 3;
  values[knight] = 3;
  values[pawn] = 1;

}

int AlphaScore::get_score(const Board &board, Side side, bool is_our_turn) {

  Board copy_board(board);

  if ( copy_board.is_check_mate(side) ) {

    return is_our_turn ? US_CHECK_MATE_SCORE : THEM_CHECK_MATE_SCORE;
  }

  if ( copy_board.is_stale_mate(side) )
    return is_our_turn ? US_STALE_MATE_SCORE : THEM_STALE_MATE_SCORE;;

  return get_score_board(copy_board, side);
}

int AlphaScore::get_score_board(Board board, Side side) {

  int score = 0;

  for (int x=0; x<8; x++)
    for (int y=0; y<8; y++) {

      Piece *piece = board.get_piece(x, y);

      if (piece == NULL)
	continue;

      score += score_counting_pieces(piece, side);
      score += score_covering_pieces(piece, board, x, y);
    }

  return score;
} 

int AlphaScore::score_counting_pieces(Piece *piece, Side side) {

  if ( piece == NULL )
    return 0;

  return values[piece->get_kind()] * (side == piece->get_side() ? 1 : -1);
}

int AlphaScore::score_covering_pieces(Piece *piece, Board &board, int x, int y) {

  if ( piece == NULL )
    return 0;

  Position pos(x,y);
  int score = 0;

  vector<Position> covers;
  piece->covers(covers, board, pos);

  for (vector<Position>::iterator it = covers.begin(); it != covers.end(); ++it) {
    if ( board.get_piece(*it) != NULL ) 
      score += POINTS_FOR_COVERING;
  }

  return score;
}
