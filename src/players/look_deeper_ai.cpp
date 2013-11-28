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

  return move; 

}


int LookDeeperAI::get_recursive_score(Board board, int current_depth, Side side, bool is_our_turn) {

  Candidates candidates(_width, is_our_turn);

  // get_candidates(&candidates, board, is_our_side);
  
  
  throw "not yet implemented!";

  return 0;
}

void LookDeeperAI::get_candidates(Candidates &candidates, Board &board, Side side) {

}

void LookDeeperAI::update_candidates_scores(Candidates &candidates, int current_depth, Side side, bool is_our_turn) {


} 
