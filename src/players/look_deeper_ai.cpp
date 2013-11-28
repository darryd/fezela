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

using namespace std; 

LookDeeperAI::LookDeeperAI(ScoreKeeper *score_keeper, int width, int depth) : _width(width), _depth(depth) {

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

  Move move;

  throw "not yet implemented.";

  cout << "we'll use " << _depth << " when we start doing recursion..." << endl;



  return move; 
}

int LookDeeperAI::get_recursive_score(Board board, int current_depth, Side side, bool is_our_turn) {

  Candidates candidates(_width, is_our_turn);
  int recursive_score;

  get_candidates(candidates, board, side, is_our_turn);

  if ( candidates.get_total_candidates() == 0 ) {

    // This means check or stale mate
    // TODO: thinking about this.... 
    return _score_keeper->get_score(board, side, is_our_turn);
  }
  
  if ( current_depth > 0 ) 
    // update_candidate_scores(..., ...);
    ;
  

  candidates.get_winner(&recursive_score);

  cerr << __FUNCTION__ <<  " not yet implemented!" << endl;

  return recursive_score;
}

void LookDeeperAI::get_candidates(Candidates &candidates, Board &board, Side side, bool is_our_turn) {

  vector<Board> board_moves = board.get_board_moves(side);

  for ( vector<Board>::iterator it = board_moves.begin(); it != board_moves.end(); ++it )
    candidates.add(*it, _score_keeper->get_score(board, side, is_our_turn));
}

void LookDeeperAI::update_candidates_scores(Candidates &candidates, int current_depth, Side side, bool is_our_turn) {

  int total_candidates = candidates.get_total_candidates();

  for (int i=0; i < total_candidates; i++) {
    
    int recursive_score;

    recursive_score = get_recursive_score(candidates.get_board(i), current_depth - 1, side, is_our_turn);
    candidates.set_score(i, recursive_score);
  }
} 
