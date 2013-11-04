
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
 * class: RandomAI
 *
 * This is an AI that plays chess by just making random (yet legal) moves. Probably the worst chess AI
 * ever, but this serves as an example.
 *
 *------------------------------------------------------------------------------------------------------*/

#include "../players.h"
#include <cstdlib>
#include <ctime>

using namespace std; 

RandomAI::RandomAI() {

  // Initialize random seed.

  srand(time(NULL));
}

Move RandomAI::play_turn(const Board &board, Side side) {

  Board copy_board(board);
  int i;

  vector<Board> board_moves = copy_board.get_board_moves(side);

  i = rand() % board_moves.size();

  return board_moves[i].get_move();
}
