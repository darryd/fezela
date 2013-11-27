
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
#include "score.h"

/*-------------------------------------------------------------------------------------------------*/

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

/*-------------------------------------------------------------------------------------------------*/

class KeyboardPlayer : public Player {

  public:
    virtual Move play_turn(const Board &board, Side side);
  private:
    Move str_to_move(std::string str_move);
    Position str_to_position(std::string str_pos);
    bool is_valid_str_move(std::string str_move);
    bool is_valid_str_pos(std::string str_pos);
    void check_promotion(Move &move, Side side); 
};

/*-------------------------------------------------------------------------------------------------*/

#define DEFAULT_MAX_CANDIDATES 5

struct BoardScore {
  Board board;
  int score;
};
  
class Candidates {

  public:
    Candidates(size_t max_candidates = DEFAULT_MAX_CANDIDATES, bool get_max = true);
    ~Candidates();
    Board select();
    void add(Board &board, int score);
  private:
    typedef int (* compare_function)(int a, int b);
    size_t _total_candidates;
    size_t _max_candidates;
    compare_function _cmp_f;
    BoardScore *_candidates;
};

class LookDeeperAI : public Player {

  public:
    LookDeeperAI(ScoreKeeper *score_keeper = NULL, int width=1, int depth=1);
    ~LookDeeperAI();
    virtual Move play_turn(const Board &board, Side side);
  private:
    int _width;
    int _depth;
    ScoreKeeper *_score_keeper;
    bool _need_to_delete_score_keeper;
    Board r_get_best_score(Board board, int current_depth, bool isOurSide);
};

/*-------------------------------------------------------------------------------------------------*/

