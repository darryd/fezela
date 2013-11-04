
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
 * class: CastlingInfo 
 *    
 * written by Darry Danzig
 * ------------------------------------------------------------------------------------------------------*/

#include "chess.h"

/*--------------------------------------------------------------------------------------------------------
 * constructor
 *
 * ------------------------------------------------------------------------------------------------------*/
CastlingInfo::CastlingInfo(): did_king_move(false), did_k_rook_move(false), did_q_rook_move(false) {}

void CastlingInfo::clear() {

  did_king_move = false;
  did_k_rook_move = false;
  did_q_rook_move = false;
}

/*--------------------------------------------------------------------------------------------------------
 * method: did_rook_move
 *
 * With this method you can specify with move you are talking about rather than access the
 * did_k_rook_move or did_q_rook_move directly.
 *
 * ------------------------------------------------------------------------------------------------------*/
bool CastlingInfo::did_rook_move(RookKind rook_kind) {
  return rook_kind == kings_rook ? did_k_rook_move : did_q_rook_move;
}

/*--------------------------------------------------------------------------------------------------------
 * method: get_rook_before_castling_position
 *
 * This gives you the location of a rook before one castles.
 * 
 * ------------------------------------------------------------------------------------------------------*/
Position CastlingInfo::get_rook_before_castling_position(Side side, RookKind rook_kind) {

  Position pos;

  pos.x = rook_kind == queens_rook ? 0 : TOTAL_ROWS - 1;
  pos.y = side == white ? 0 : TOTAL_ROWS - 1;

  return pos;
}

/*---------------------------------------------------------------------------------------------------------
 * method: get_rook_after_castling_position(Side side, RookKind rook_kind
 *
 * This gives the location of a rook after one castles.
 *
 * -------------------------------------------------------------------------------------------------------*/
Position CastlingInfo::get_rook_after_castling_position(Side side, RookKind rook_kind) {

  Position pos;

  pos.x = rook_kind == queens_rook ? 3 : 5;
  pos.y = side == white ? 0 : TOTAL_ROWS - 1;

  return pos;
}

/*--------------------------------------------------------------------------------------------------------
 * method: get_king_before_castling_position
 *
 * This returns where the king is located before it castles (assuming it hasn't moved from this 
 * original position).
 * -----------------------------------------------------------------------------------------------------*/
Position CastlingInfo::get_king_before_castling_position(Side side) {

  Position pos;

  pos.x = 4;
  pos.y = side == white ? 0 : TOTAL_ROWS - 1;

  return pos;
}

/*--------------------------------------------------------------------------------------------------------
 * method: get_king_after_castle_position
 *
 * This returns the king's final location after it has castled.
 * -----------------------------------------------------------------------------------------------------*/
Position CastlingInfo::get_king_after_castling_position(Side side, RookKind rook_kind) {

  Position king_pos = get_king_before_castling_position(side);

  Position direction = rook_kind == queens_rook ? Position(-2, 0) : Position(2, 0);

  return  king_pos + direction;
}

/*--------------------------------------------------------------------------------------------------------
 * method: is_clear_between_king_and_rook
 *
 * In order to castle there can be no pieces in between the king and the rook to be castled with.
 *
 * This returns true if there are no pieces between the king and the rook.
 * ------------------------------------------------------------------------------------------------------*/
bool CastlingInfo::is_clear_between_king_and_rook(Board &board, Side side, RookKind rook_kind) {

  Position king_pos = get_king_before_castling_position(side);
  Position rook_pos = get_rook_before_castling_position(side, rook_kind);

  int dir = king_pos.x < rook_pos.x ? 1 : -1; // Direction going from king to rook.

  // Check all squares between the king and the rook for the presence of a piece.
  int start, end;

  start = 1 + Utl::min(king_pos.x, rook_pos.x);
  end = Utl::max(king_pos.x, rook_pos.x);

  for (int x = start; x < end; x++)
    if ( board.get_piece(x, king_pos.y) != NULL)
      return false;

  return true;
}

/*--------------------------------------------------------------------------------------------------------
 * method: is_safe_to_castle
 *
 * To castle the king may not be in check, it may not pass through check, and may not end up in check.
 *
 * Returns whether true if the above requirement is satisfied.
 * ------------------------------------------------------------------------------------------------------*/
bool CastlingInfo::is_safe_to_castle(Board &board, Side side, RookKind rook_kind) { 

  Board copy_board(board);
  King king(side);

  // With the copied board, 'board_copy', put extra kings on the board. Then check whether this puts 
  // 'side' into check.
  
  Position king_before_pos = get_king_before_castling_position(side);
  Position king_after_pos = get_king_after_castling_position(side, rook_kind);

  int start, end;

  start = Utl::min(king_before_pos.x, king_after_pos.x);
  end = Utl::max(king_before_pos.x, king_after_pos.x);

  for (int x = start; x <= end; x++)
    copy_board.set_piece(&king, x, king_before_pos.y);

  return !copy_board.is_in_check(side);
}

/*--------------------------------------------------------------------------------------------------------
 * method: CastlingInfo::can_castle
 *
 * Parameters
 * Side side: The side that wants to castle
 * RookKind rook_kind: the rook that will be doing the castling.
 *
 * Returns true if it is legal to castle.
 *
 * -------------------------------------------------------------------------------------------------------*/
bool CastlingInfo::can_castle(Board &board, Side side, RookKind rook_kind) {

  // Strategy: do a series of checks. As soon as it fails return false.

  // Determine where the rook (doing the castling) is located.

  Position rook_pos = CastlingInfo::get_rook_before_castling_position(side, rook_kind);

  Piece *piece = board.get_piece(rook_pos);
  if ( piece == NULL ) 
    return false;

  // Check that the king and rook have not moved.
  if ( did_king_move || did_rook_move(rook_kind) )
    return false;

  // The rook might not have moved, but it might have been eaten by another piece.
  // Check if the rook is there.
  if ( piece->get_side() != side || piece->get_kind() != rook) {
   
    Rook *rook = (Rook *) piece;

    if ( rook->get_rook_kind() != rook_kind )
      return false;
  }

  // Check that there's a clear path between king and the rook
  if ( !is_clear_between_king_and_rook(board, side, rook_kind) )
    return false;
  
  // Check that the king will not be check
  if ( !is_safe_to_castle(board, side, rook_kind) )
      return false;

  return true;
}
