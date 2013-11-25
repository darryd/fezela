
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

// This is experimental.

#include "chess.h"


using namespace std;

Subscriber::Subscriber():_seq_no(0) {
  pthread_mutex_init(&_mutex, NULL);
}

/*--------------------------------------------------------------------------------------------------------*/

void Subscriber::notification(GameData game_data, int seq_no) {

  pthread_mutex_lock(&_mutex);

  if ( _seq_no == seq_no ) {
    do_work(game_data);
    _seq_no++;
    process_waiting_room();
  }
  else {
    _waiting_room[seq_no] = game_data;
  }

  pthread_mutex_unlock(&_mutex);
}

/*--------------------------------------------------------------------------------------------------------*/

void Subscriber::process_waiting_room() {

  map<int, GameData>::iterator it;

  while ( (it = _waiting_room.find(_seq_no)) != _waiting_room.end() ) {

    do_work(it->second);
    _waiting_room.erase(it);

    _seq_no++;
  }

}
