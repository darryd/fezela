
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
 * file: players.h
 *
 * The file is to list all the different Players.
 * 
 * ------------------------------------------------------------------------------------------------------*/

#pragma once

#include "chess.h"

class RandomAI : public Player {

  public:
    RandomAI();
    virtual Move play_turn(const Board &board, Side side);

};

class SimplyCountingAI : public Player {

  public:
    SimplyCountingAI();
    virtual Move play_turn(const Board &board, Side side);
    Board get_best_score(std::vector<Board> &board_moves, Side side);
    int score(Board &board, Side side);
  private:
    std::map<Kind, int > values;
};

class CountingAndCheckingAI : public Player {

  public:
    CountingAndCheckingAI();
    virtual Move play_turn(const Board &board, Side side);
    Board get_best_score(std::vector<Board> &board_moves, Side side);
    int score(Board &board, Side side);
  private:
    std::map<Kind, int > values;
};

class KeyboardPlayer : public Player {

  public:
    virtual Move play_turn(const Board &board, Side side);
  private:
    Move str_to_move(std::string str_move);
    Position str_to_position(std::string str_pos);
    bool is_valid_str_move(std::string str_move);
    bool is_valid_str_pos(std::string str_pos);
};
