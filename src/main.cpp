
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
#include "chess.h"
#include <unistd.h>

#include "players.h"

using namespace std;

int main() {

  RandomAI random_ai;
  SimplyCountingAI simply_counting_ai;
  CountingAndCheckingAI counting_and_checking_ai;
  KeyboardPlayer keyboard;
//  LookDeeperAI look_deeper_ai(NULL, 5, 2);

  /*
  Game game(&counting_and_checking_ai, &look_deeper_ai);

  game.play();
  

  return 0;
*/
  Board board;
  board.set();

  Candidates candidates(5, true);

  for (int i=0; i<100; i++)
    candidates.add(board, i);

  candidates.print();

  int score;

  candidates.get_winner(&score);

  cout << "winning score: " << score << endl;

}


