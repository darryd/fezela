
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
 * class: CountingAndCheckingAi
 *
 * This ai, isn't very smart. It just makes moves based on counting the pieces on the board.
 *
 *------------------------------------------------------------------------------------------------------*/

#include "../players.h"
#include <cstdlib>
#include <ctime>
#include <map>
#include <algorithm>

using namespace std; 

CountingAndCheckingAI::CountingAndCheckingAI() {

  values[king] = 0;
  values[queen] = 9;
  values[rook] = 5;
  values[bishop] = 3;
  values[knight] = 3;
  values[pawn] = 1;

  std::srand ( unsigned ( std::time(0) ) );

}

int CountingAndCheckingAI::score(Board &board, Side side) {

  int score = 0;

  for (int x=0; x<8; x++)
    for (int y=0; y<8; y++) {

      Piece *piece = board.get_piece(x, y);

      if (piece == NULL)
	continue;

      score += values[piece->get_kind()] * (side == piece->get_side() ? 1 : -1);
    }
  return score;
}

Move CountingAndCheckingAI::play_turn(const Board &board, Side side) {

  Board copy_board(board);
  

  vector<Board> board_moves = copy_board.get_board_moves(side);

  if ( board_moves.size() == 0 )
    throw "There are no moves.";

  if ( board_moves.size() == 1) 
    return board_moves[0].get_move();

  return get_best_score(board_moves, side).get_move();
}

int myrandom (int i) { return std::rand()%i;}

// Precondition: the vector 'board_moves' is not empty.
Board CountingAndCheckingAI::get_best_score(vector<Board> &board_moves, Side side) {

  std::random_shuffle ( board_moves.begin(), board_moves.end(), myrandom);


  vector<Board>::iterator i = board_moves.begin();
  int max_score = score(*i, side);
  Board max_board = *i;

  while ( true ) {

    ++i;
    if ( i == board_moves.end() )
      break;

    int tmp_score = score(*i, side);

    if ( tmp_score > max_score ) {

      max_score = tmp_score;
      max_board = *i;

    }

  }
  
  return max_board;
}
