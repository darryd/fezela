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

  _compare_f = get_max ? Utl::max : Utl::min;
}

Candidates::~Candidates() {

  free(_candidates);
}

// Precondition: there are candidates.
Board Candidates::select() {

  BoardScore selected;

  if ( _total_candidates >= 1 )
    selected = _candidates[0];

  for (size_t i=0; i < _total_candidates; i++) 
    if ( _compare_f ( _candidates->score, selected.score) == _candidates->score )
      selected = _candidates[i];

  return selected.board;
}

void Candidates::add(Board &board, int score) {

  if ( _total_candidates < _max_candidates ) {

    _candidates[_total_candidates].board = board;
    _candidates[_total_candidates].score = score;

    _total_candidates++;
  }
  else {

    for (size_t i = 0; i < _max_candidates; i++)
      if ( _compare_f(score, _candidates[i].score) == score ) {
	
	_candidates[i].board = board;
	_candidates[i].score = score;
      }

  }
}
