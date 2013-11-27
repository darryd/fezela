#include "../players.h"


LookDeeper::LookDeeper(ScoreKeeper *score_keeper, int width, int depth) : _width(width), _depth(depth) {

  if ( score_keeper == NULL ) {
    _need_to_delete_score_keeper = true;
    _score_keeper = new AlphaScore;
  }
  else {
    _need_to_delete_score_keeper = false;
    _score_keeper = score_keeper;
  }
}

LookDeeper::~LookDeeper() {

  if ( _need_to_delete_score_keeper )
    delete _score_keeper;
}

Move LookDeeper::play_turn(const Board &board, Side side) {

  Move move;

  return move;

}
