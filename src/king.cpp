
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
 * class King
 *
 * written by Darry Danzig
 * ------------------------------------------------------------------------------------------------------*/

#include "chess.h"
using namespace std;

void King::covers(vector<Position> &list_covers, Board &board, Position &pos) {

  Utl::push_if_is_on_board(list_covers, pos + Position(0, 1));
  Utl::push_if_is_on_board(list_covers, pos + Position(1, 1));
  Utl::push_if_is_on_board(list_covers, pos + Position(1, 0));
  Utl::push_if_is_on_board(list_covers, pos + Position(1, -1));
  Utl::push_if_is_on_board(list_covers, pos + Position(0, -1));
  Utl::push_if_is_on_board(list_covers, pos + Position(-1, -1));
  Utl::push_if_is_on_board(list_covers, pos + Position(-1, 0));
  Utl::push_if_is_on_board(list_covers, pos + Position(-1, 1));

}
void King::moves(vector<Board> &list_board_moves, Board &board, Position &pos) {

  vector<Position> list_covers;

  covers(list_covers, board, pos);
  default_moves(list_board_moves, list_covers, board, pos);

  // Do castling if possible.
  castle(list_board_moves, board);

}

void King::castle(vector<Board> &list_board_moves, Board &board) {

  vector<RookKind> rook_kinds;

  rook_kinds.push_back(kings_rook);
  rook_kinds.push_back(queens_rook);

  for (vector<RookKind>::iterator i = rook_kinds.begin(); i != rook_kinds.end(); ++i) {

    CastlingInfo *info = board.get_castling_info(get_side());
    RookKind rook_kind = *i;

    // Castle if it is legal now.
    if ( info->can_castle(board, get_side(), rook_kind) ) {

      // Castle
      Move castling_move;

      castling_move.old_pos = CastlingInfo::get_king_before_castling_position(get_side());
      castling_move.new_pos = CastlingInfo::get_king_after_castling_position(get_side(), rook_kind);

      castling_move.is_castling = true;
      castling_move.extra_old_pos = CastlingInfo::get_rook_before_castling_position(get_side(), rook_kind);
      castling_move.extra_new_pos = CastlingInfo::get_rook_after_castling_position(get_side(), rook_kind);

      Board castling_board(board);
      castling_board.move(castling_move);
      list_board_moves.push_back(castling_board);
    }

  }

}
