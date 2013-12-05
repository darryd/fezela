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
#include <stdlib.h>

using namespace std;

Candidates::Candidates(size_t max_candidates, bool get_max) :_total_candidates(0), _max_candidates(max_candidates) {

  _candidates = (BoardScore *) malloc (_max_candidates * sizeof(BoardScore));

  if ( _candidates == NULL ) {
    cerr << __FILE__ << endl;
    cerr << "Candidates::Candidates(): Error. No memory!" << endl; 
    cerr << "Exiting..." << endl;
    exit(1);
  }

  _compare_winner_f = get_max ? Utl::max : Utl::min;
  _compare_loser_f  = get_max ? Utl::min : Utl::max;
}

Candidates::~Candidates() {

  free(_candidates);
}

// Precondition: there are candidates.
BoardScore Candidates::get_winner() {
  return get_candidate(_compare_winner_f);
}

// Precondition: there are candidates.
BoardScore Candidates::get_loser() {
  return get_candidate(_compare_loser_f);
}

// Precondition: there are candidates.
BoardScore Candidates::get_candidate(Candidates::CompareFunc cmp) {
 
  BoardScore the_candidate;

  if ( _total_candidates >= 1 ) {
    the_candidate = _candidates[0];
    the_candidate.index = 0;
  }

  for (size_t i=1; i < _total_candidates; i++) 
    if ( cmp ( _candidates[i].score, the_candidate.score) == _candidates[i].score ) {
      the_candidate = _candidates[i];
      the_candidate.index = i;
    }

  return the_candidate;
}

void Candidates::nominate(Board board, int score) {

  if ( _total_candidates < _max_candidates ) {

    _candidates[_total_candidates].board = board;
    _candidates[_total_candidates].score = score;

    _total_candidates++;
  }
  else {

    BoardScore lowest_candidate;
    lowest_candidate = get_loser();

    if ( _compare_winner_f(score, lowest_candidate.score ) == score) {

      _candidates[lowest_candidate.index].board = board;
      _candidates[lowest_candidate.index].score = score;
    }
  }
}

Board Candidates::get_board(size_t index) {

  if ( index >= _total_candidates )
    throw "Candidates::get_board(): index too big.";

  return _candidates[index].board;
}

void Candidates::set_score(size_t index, int score) {

  if ( index >=  _total_candidates ) 
    throw "Candidates::set_score(): index too big.";

  _candidates[index].score = score;
}

void Candidates::print() {

  int width = 25;
  int row;
  int col;
  int margin = 10;

  for (size_t i=0; i < _total_candidates; i++) {

    row = margin;

    col = margin + width * i;

    Utl::move_cursor(row, col);
    cout << i << ":";
    Utl::move_cursor(++row, col);
    cout << "Score = " << _candidates[i].score;
    Utl::move_cursor(++row, col);
    _candidates[i].board.print(row, col);
  }
}
