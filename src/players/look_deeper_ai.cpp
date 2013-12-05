
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

LookDeeperAI::LookDeeperAI(int width, int depth, ScoreKeeper *score_keeper) : AI(), _width(width), _depth(depth) {

  if ( score_keeper == NULL ) {
    _need_to_delete_score_keeper = true;
    _score_keeper = new AlphaScore;
  }
  else {
    _need_to_delete_score_keeper = false;
    _score_keeper = score_keeper;
  }
}

LookDeeperAI::~LookDeeperAI() {

  if ( _need_to_delete_score_keeper )
    delete _score_keeper;
}

Move LookDeeperAI::play_turn(const Board &board, Side side) {

  Candidates candidates(_width, true);
  get_candidates(candidates, board, side, true);

  if ( candidates.get_total_candidates() == 0 )
    throw "LookDeeperAI::play_turn(): No moves available.";

  BoardScore winner = helper_get_recursive_score(candidates, _depth, side, true);

  return winner.board.get_move();
}

void LookDeeperAI::get_candidates(Candidates &candidates, Board board, Side side, bool is_our_turn) {

  vector<Board> board_moves = board.get_board_moves(side);
  random_shuffle( board_moves.begin(), board_moves.end(), AI::random);

  for (vector<Board>::iterator it = board_moves.begin(); it != board_moves.end(); ++it) {
    candidates.nominate( *it, _score_keeper->get_score(*it, side, is_our_turn));
  }
}

BoardScore LookDeeperAI::helper_get_recursive_score(Candidates &candidates, int depth, Side side, bool is_our_turn) {

 int total_candidates = candidates.get_total_candidates(); 
 BoardScore winner;

 for (int i=0; i < total_candidates; i++) {

   Board board = candidates.get_board(i);
   int recursive_score = get_recursive_score(depth - 1, board, side, is_our_turn);

   candidates.set_score(i, recursive_score);
 }

 return candidates.get_winner();
}

int LookDeeperAI::get_recursive_score(int depth, Board board, Side side, bool is_our_turn) {

  if ( depth == 0 || board.is_game_over(side) ) 
    return _score_keeper->get_score(board, side, is_our_turn);

  else {

    Side opponent = Utl::opposite(side);

    Candidates candidates(_width, !is_our_turn);
    get_candidates(candidates, board, opponent, !is_our_turn);

    return helper_get_recursive_score(candidates, depth, opponent, !is_our_turn).score;
  }
}
