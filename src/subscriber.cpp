#include "chess.h"

using namespace std;

Subscriber::Subscriber():_seq_no(0) {
  pthread_mutex_init(&_mutex, NULL);
}

void Subscriber::notification(Board board, int seq_no) {

  pthread_mutex_lock(&_mutex);

  if ( _seq_no == seq_no ) {
    _seq_no++;
    do_work(board);
    process_waiting_room();
  }
  else {
    _waiting_room[seq_no] = board;
  }

  pthread_mutex_unlock(&_mutex);
}

void Subscriber::process_waiting_room() {

  map<int, Board>::iterator it;

  while ( (it = _waiting_room.find(_seq_no)) != _waiting_room.end() ) {

    do_work(it->second);
    _waiting_room.erase(it);

    _seq_no++;
  }

}
