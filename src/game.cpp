
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
 * class: Game
 *
 * written by Darry Danzig
 * ------------------------------------------------------------------------------------------------------*/
#include <unistd.h>
#include "chess.h"
#include <stdlib.h>
#include <pthread.h>

using namespace std;

struct SubscriberData {
  Subscriber *subscriber;
  GameData data;
  int seq_no;
};

struct NotifyData {
  Game *ptr_game;
  GameData data;
  int seq_no;
};

Game::Game(Player *white_player, Player *black_player): _turn(white) {

  _board.set();

  _players[white] = white_player;
  _players[black] = black_player;

  cout << "\33[2J\33[;H";
  _board.print();
  //notify(_board, 0);
}

void Game::play() {


  int seq_num = 1;

  do {

    Board copy_board(_board);
    Move move;

    while (true) {

      move = _players[_turn]->play_turn(copy_board, _turn);

#ifndef DEBUG_ALLOW_ILLEGAL_MOVES
      if ( copy_board.is_valid_move(move, _turn) )
#endif
	break;

      cout << "Invalid move!" << endl;
    }

    _board.move(move);

    cout << "\33[;H";
    _board.print();
    cout << "                               " << endl;
    //notify(_board, seq_num++ );

    _turn = Utl::opposite(_turn);

    //sleep(1);
  } while ( ! _board.is_game_over(_turn) );
}

// Experimental
void Game::subscribe(Subscriber *subscriber) {
  _subscribers.push_back(subscriber);
}

void *do_notify_subscriber(void *ptr) {

  SubscriberData *data = (SubscriberData *) ptr;

  data->subscriber->notification(data->data, data->seq_no);

  free(ptr);
  return NULL;
}

// Experimental
void *do_notify(void *ptr) {

  NotifyData *n_data = (NotifyData *) ptr;

  for (vector<Subscriber*>::iterator i = n_data->ptr_game->_subscribers.begin(); i != n_data->ptr_game->_subscribers.end(); ++i) {

    SubscriberData *data = (SubscriberData *) malloc(sizeof(SubscriberData));

    if (data == NULL) {
      cerr << "Error no memory." << endl;
      return NULL;
    }

    data->subscriber = *i;
    data->data = n_data->data;
    data->seq_no = n_data->seq_no;

    pthread_t thread;
    pthread_create(&thread, NULL, do_notify_subscriber, (void *) data);
  }

  free(ptr);
  return NULL;
}

// Experimental
void Game::notify (Board board, int seq_no) {
/*
  NotifyData *n_data = (NotifyData *) malloc (sizeof (NotifyData));

  if (n_data == NULL) {
    cerr << "Error no memory." << endl;
    return;
  }

  n_data->ptr_game = this;
  n_data->data.board = board;
  n_data->data.seq_no = seq_no;
  n_data->seq_no = seq_no;


  pthread_t thread;
  pthread_create(&thread, NULL, do_notify, (void *) n_data);
  */
}
