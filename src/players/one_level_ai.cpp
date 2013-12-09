
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
#include "../players.h"
#include <algorithm>

using namespace std; 

Move OneLevelAI::play_turn(const Board &board, Side side) {

  Board copy_board(board);

  vector<Board> board_moves = copy_board.get_board_moves(side);
  random_shuffle( board_moves.begin(), board_moves.end(), AI::random);

  return get_best_move(board_moves, side).get_move();
}

Board OneLevelAI::get_best_move(vector<Board> &board_moves, Side side) {

  Candidates candidates(1, true);

  for (vector<Board>::iterator it = board_moves.begin(); it != board_moves.end(); ++it)
    candidates.nominate(*it, _score_keeper.get_score(*it, side, true));

  return candidates.get_winner().board;

}
