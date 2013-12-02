
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
 * class: Board
 *    
 * written by Darry Danzig
 *
 * ------------------------------------------------------------------------------------------------------*/

#include "chess.h"
#include <algorithm> 
#include <cstring>
#include "global_pieces.h"

#define yes_or_no(x) ((x) ? "yes" : "no")

using namespace std;

/*--------------------------------------------------------------------------------------------------------
 * constructor
 *
 * ------------------------------------------------------------------------------------------------------*/

Board::Board() {
  clear();
}

/*--------------------------------------------------------------------------------------------------------
 * method Board::is_in_check
 *
 * Returns true if side side is in check.
 *
 * Parameters:
 * side: The side to check whether it is in check or not.
 *
 * ------------------------------------------------------------------------------------------------------*/
bool Board::is_in_check(Side side) {

  vector<Position> list_covers;
  Side opponent = Utl::opposite(side);

  for (int x=0; x<TOTAL_ROWS; x++)
    for (int y=0; y<TOTAL_ROWS; y++) {

      Position pos = Position(x, y);
      list_covers.clear();

      // Step 1: Get covers for piece if it is an oppenent.
      Piece *piece = get_piece(x, y);
      if ( piece != NULL && piece->get_side() == opponent ) {

	piece->covers(list_covers, *this, pos);
      }

      // Step 2: See if the opponent covers our king.
      // Return early if in check by that piece.
      for (vector<Position>::iterator i = list_covers.begin(); i != list_covers.end(); ++i) {

	Piece *piece = get_piece(i->x, i->y);

	if ( piece != NULL && piece->get_side() == side && piece->get_kind() == king  )
	  return true;
      }	
    }
  return false;
}

/*--------------------------------------------------------------------------------------------------------
 * method Board::is_mate
 *
 * This tells you if the side is in some kind of mate. 
 *
 * Of course, the assumption is that you are calling this on a side who's turn it is.
 *
 * -----------------------------------------------------------------------------------------------------*/
bool Board::is_mate(Side side) {

  vector<Board> board_moves = get_board_moves(side);

  return board_moves.size() == 0;
}

/*--------------------------------------------------------------------------------------------------------
 * method Board::clear
 *
 * Clears all the pieces off the board.
 *
 * ------------------------------------------------------------------------------------------------------*/
void Board::clear() {

  _last_move_count = 0;

  for (int i=0; i<TOTAL_ROWS; i++)
    memset(_pieces[i], 0, sizeof(Piece*) * TOTAL_ROWS);

  _w_castling_info.clear();
  _b_castling_info.clear();

}

/*--------------------------------------------------------------------------------------------------------
 * method Board::set
 *
 * Sets the board with the pieces in their starting positions.
 *
 * -----------------------------------------------------------------------------------------------------*/
void Board::set() {

  clear();

  // Set the white side
  
  _pieces[0][0] = &w_q_rook;
  _pieces[1][0] = &w_knight;
  _pieces[2][0] = &w_bishop;
  _pieces[3][0] = &w_queen;
  _pieces[4][0] = &w_king;
  _pieces[5][0] = &w_bishop;
  _pieces[6][0] = &w_knight;
  _pieces[7][0] = &w_k_rook;

  for (int x=0; x<TOTAL_ROWS; x++)
    _pieces[x][1] = &w_pawn;

  // Set black side
  
  _pieces[0][7] = &b_q_rook;
  _pieces[1][7] = &b_knight;
  _pieces[2][7] = &b_bishop;
  _pieces[3][7] = &b_queen; 
  _pieces[4][7] = &b_king; 
  _pieces[5][7] = &b_bishop;
  _pieces[6][7] = &b_knight;
  _pieces[7][7] = &b_k_rook;

  for (int x=0; x<TOTAL_ROWS; x++)
    _pieces[x][6] = &b_pawn;
}

/*--------------------------------------------------------------------------------------------------------
 * method Board::print
 *
 * Displays the board on the terminal using ascii characters and vt100 emulation codes.
 *
 * ------------------------------------------------------------------------------------------------------*/
void Board::print(int row, int col) {

  Utl::move_cursor(row, col);

  for (int y=7; y>=0; y--) {

    cout << y + 1 << "  ";

    for (int x=0; x<TOTAL_ROWS; x++) {

      Piece *piece = get_piece(x, y);

      if ( piece == NULL )  {

	// No piece is on the square

	// '+' is a black square
	// '.' is a white square
	char ch = (x % 2 == y % 2) ? '+' : '.';

	cout << ch << ' ';
      }
      else {
	// A piece is on the square
	cout << piece->get_ascii_code() << ' ';
      }
    }
    //cout << endl;
    Utl::move_cursor(++row, col);
  }
  Utl::move_cursor(++row, col);
  cout << "   a b c d e f g h";// << endl << endl;

  Utl::move_cursor(++row, col);
  Utl::move_cursor(++row, col);

  print_check(white);
  print_check(black);

  cout.flush();
}

/*--------------------------------------------------------------------------------------------------------
 * method Board::print_check
 *
 * This function will display infomation if 'side' is in check. Otherwise, nothing is displayed.
 *
 * ------------------------------------------------------------------------------------------------------*/
void Board::print_check(Side side, int row, int col) {

  if ( is_check_mate(side) ) {
    cout << CODE_IN_CHECK << Utl::str(side) << " is in checkmate!" << CODE_RESET;
    Utl::move_cursor(++row, col);
  }

  else if ( is_stale_mate(side) ) {
    cout << CODE_IN_CHECK << Utl::str(side) << " is in stalemate!" << endl << CODE_RESET;
    Utl::move_cursor(++row, col);
  }

  else if ( is_in_check(side) ) {
    cout << CODE_IN_CHECK << Utl::str(side) << " is in check!" << endl << CODE_RESET;
    Utl::move_cursor(++row, col);
  }

}

/*-------------------------------------------------------------------------------------------------------
 * method Board::update_castling_info
 *
 * This is to keep track if king or a rook's king or queen's rook moved in the game.
 * This is method is called by Board::move() when a piece moves.
 *
 * ------------------------------------------------------------------------------------------------------*/
void Board::update_castling_info(Piece *piece) {

  if ( piece->get_kind() == king ) 
    get_castling_info(piece->get_side())->did_king_move = true;

  if ( piece->get_kind() == rook ) {

    Rook *rook = (Rook *) piece;

    if ( rook->get_rook_kind() == kings_rook )
      get_castling_info(piece->get_side())->did_k_rook_move = true;

    if ( rook->get_rook_kind() == queens_rook )
      get_castling_info(piece->get_side())->did_q_rook_move = true;
  }
}

/*--------------------------------------------------------------------------------------------------------
 * method Board::move
 *
 * Moves a piece on the board (or it can move two if it is castling).
 *
 * Side effects:
 *   Updates castling info
 *   Remembers the move
 *
 * ------------------------------------------------------------------------------------------------------*/
void Board::move(Move &move) {

  Piece *piece;

  _last_move = move;

  piece = get_piece(move.old_pos);
  if ( piece == NULL)
    return;

  _last_move_count++; // TODO: This is going to be redesigned.

  set_piece(piece, move.new_pos);
  set_piece(NULL, move.old_pos);

  update_castling_info(piece);

  //---------------castling
 
  if ( move.is_castling ) {

    piece = get_piece(move.extra_old_pos);

    if ( piece == NULL ) 
      return;

    update_castling_info(piece);
    set_piece(piece, move.extra_new_pos);
    set_piece(NULL, move.extra_old_pos);
  }

  //--------------- pawn promotion
  if (move.promotion) 
    set_piece(move.promotion, move.new_pos);

  //--------------- en passant
  if (move.is_en_passant) {

    piece = get_piece(move.new_pos);
    if ( piece == NULL ) 
      return;

    int dir = -1 * Utl::direction(piece->get_side());
    set_piece(NULL, move.new_pos.x, move.new_pos.y + dir);
  }
}

/*--------------------------------------------------------------------------------------------------------
 * method get_board_moves
 *
 * Returns a vector of board moves that can be made by side 'side'.
 *
 * ------------------------------------------------------------------------------------------------------*/
vector<Board> Board::get_board_moves(Side side) {

  vector<Board> board_moves;

  for (int x=0; x<TOTAL_ROWS; x++)
    for (int y=0; y<TOTAL_ROWS; y++) {

      // Check moves for all pieces belonging to side.

      Piece *piece = get_piece(x, y);

      if ( piece != NULL && piece->get_side() == side ) {

	Position pos(x, y);
	piece->moves(board_moves, *this, pos);
      }
    }

  return board_moves;
}

/*--------------------------------------------------------------------------------------------------------
 * method: print_castling_info
 *
 * Prints castling statistics for 'side'.
 *
 * I wrote this method to testing purposes.
 *
 * ------------------------------------------------------------------------------------------------------*/
void Board::print_castling_info(Side side) {

  CastlingInfo *info;

  info = get_castling_info(side);

  cout << Utl::str(side) << ":" << endl;
  cout << "    king moved: " << yes_or_no(info->did_king_move) << endl;
  cout << "    king's rook moved: " << yes_or_no(info->did_rook_move(kings_rook)) << endl;
  cout << "    queen's rook moved: " << yes_or_no(info->did_rook_move(queens_rook)) << endl;
  cout << "    can castle on king's side: " << yes_or_no(info->can_castle(*this, side, kings_rook)) << endl;
  cout << "    can castle on queen's side: " << yes_or_no(info->can_castle(*this, side, queens_rook)) << endl;
}

/*--------------------------------------------------------------------------------------------------------
 * method print_castling_info
 *
 * Prints castling statistics both sides. 
 *
 * I wrote this method for testing purposes.
 *
 * ------------------------------------------------------------------------------------------------------*/
void Board::print_castling_info() {

  print_castling_info(white);
  print_castling_info(black);
}

/*--------------------------------------------------------------------------------------------------------
 * method operator==
 *
 * ------------------------------------------------------------------------------------------------------*/
bool Board::operator==(const Board &rhs) const {

  Board lhs_copy(*this);
  Board rhs_copy(rhs);

  for (int x = 0; x < TOTAL_ROWS; x++)
    for (int y = 0; y < TOTAL_ROWS; y++)
      if ( lhs_copy.get_piece(x,y) != rhs_copy.get_piece(x,y) )
	return false;

  return true;
}

/*--------------------------------------------------------------------------------------------------------
 * method is_valid_move
 *
 * ------------------------------------------------------------------------------------------------------*/

bool Board::is_valid_move(Move &move, Side side) {

  vector<Board> valid_boards = get_board_moves(side);

  Board candidate_board(*this);
  candidate_board.move(move);

  return std::find(valid_boards.begin(), valid_boards.end(), candidate_board)!= valid_boards.end();
}

/*--------------------------------------------------------------------------------------------------------
 *
 * method is_valid_move_allow_implied
 *
 * Primarily this method is intended to be used when getting a move from a user. For example, to castle, 
 * the user just moves the king. The user doesn't move rook, therefore, this makes the move implied. 
 *
 * If the move is implied (and 'update_move' is set to true), the Move structure will be updated.
 * The following changes will indicate if it is implied:
 *    - In castling: move.is_castling will be true.
 *    - In promotion: move.promotion will not be NULL.
 *    - In en passant: move.is_en_passant will be true.
 * ------------------------------------------------------------------------------------------------------*/

bool Board::is_valid_move_allow_implied(Move &move, Side side, bool update_move) {

  vector<Board> valid_boards = get_board_moves(side);

  for (vector<Board>::iterator i = valid_boards.begin(); i != valid_boards.end(); ++i) {

    Move valid_move = i->get_move();

    if ( valid_move.old_pos == move.old_pos && valid_move.new_pos == move.new_pos ) {
      
      if ( update_move )
	memcpy(&move , &valid_move, sizeof(Move));

      return true;
    }
  }

  return false;
}
