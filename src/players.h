
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
  size_t index;
};
  
class Candidates {

  public:
    Candidates(size_t max_candidates = DEFAULT_MAX_CANDIDATES, bool get_max = true);
    ~Candidates();
    BoardScore get_winner();
    void nominate(Board board, int score);
    size_t get_total_candidates() { return _total_candidates; }
    size_t get_max_candidates() { return _max_candidates; }
    Board get_board(size_t index);
    void set_score(size_t index, int score);
    void print();

  private:
    typedef int (* CompareFunc)(int a, int b);
    
    CompareFunc _compare_winner_f;
    CompareFunc _compare_loser_f;
    size_t _total_candidates;
    size_t _max_candidates;
    BoardScore *_candidates;
    BoardScore get_candidate(CompareFunc cmp);
    BoardScore get_loser();
};

class LookDeeperAI : public Player {
  public:
    LookDeeperAI(int width=1, int depth=1, ScoreKeeper *score_keeper = NULL); 
    ~LookDeeperAI();
    virtual Move play_turn(const Board &board, Side side);

    // public while i'm testing
    void get_candidates(Candidates &candidates, Board board, Side side, bool is_our_turn);

    int get_recursive_score(int depth, Board board, Side side, bool is_our_turn);
    BoardScore helper_get_recursive_score(Candidates &candidates, int depth, Side side, bool is_our_turn);
  private:
    int _width;
    int _depth;
    ScoreKeeper *_score_keeper;
    bool _need_to_delete_score_keeper;
};

#if 0
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
    int get_recursive_score(Board board, int current_depth, Side side, bool is_our_turn);
    void get_candidates(Candidates &candidates, Board &board, Side side, bool is_our_turn);
    void update_candidates_scores(Candidates &candidates, int current_depth, Side side, bool is_our_turn);
};
#endif
/*-------------------------------------------------------------------------------------------------*/

