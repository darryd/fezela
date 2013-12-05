
--[[
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
]]
-- Precondition piece is not nil
function is_piece(piece, board, side, position)


  local something_here
  local side_here

  something_here, side_here = is_occupied(board, position)

  if something_here == false or side_here ~= side then
    return false
  end
  
  return piece == board[position.row][position.column].kind.piece
  
end

function did_piece_move(piece, board, side, position)

  if not is_piece(piece, board, side, position) then
    return true
  end

  return board[position.row][position.column].has_moved
end


function did_king_move(board, side) 

  local x = 5
  local y

  if side == "w" then
    y = 1
  else
    y = 8
  end

  return did_piece_move(king, board, side, {row=y, column=x})

end

function did_kings_rook_move(board, side)
  
  local x = 8
  if side == "w" then
    y = 1
  else
    y = 8
  end

  return did_piece_move(rook, board, side, {row=y, column=x})
end

function did_queens_rook_move(board, side)

  local x = 1
  if side == "w" then 
    y = 1
  else
    y = 8
  end

  return did_piece_move(rook, board, side, {row=y, column=x})
end

-- Return true if there are no pieces between the king and the rook
function is_free_path_between_king_and_rook(board, x_rook, y_rook)

  local x_king = 5
  local i
  local step

  if x_rook < x_king then
    step = 1
  else
    step = -1
  end

  for i= x_rook + step, x_king - step, step do

    if board[y_rook][i] ~= nil then
      return false
    end
  end

  return true
end

-- To castle the king may not be in check, it may not pass through check,
-- and may not end up in check
function is_safe_to_castle(board, y_king, castle_side, side)

  local x_king = 5
  local step
  local copy = copy_board(board)


  local score 
  local white_in_check
  local black_in_check


  if castle_side == "k" then
    step = 1
  else
    step = -1
  end

  -- To determine if the king will never be in check, place kings everywhere the king
  -- will travel and use score function to see if the board is in check

  local i

  for i=step, step * 2, step do

    copy[y_king][x_king + i] = { kind = copy[y_king][x_king].kind }
     
  end

  score, white_in_check, black_in_check = get_score(copy, side)
  
  if side == "w" then
    return not white_in_check
  else
    return not black_in_check
  end

end


-- castling side is "k" or "q"
function can_castle_king(board, side, castle_side)

  local x, y
  local x_rook
  local step
  local f_rook_moved

  if side == "w" then
    y = 1
  else
    y = 8
  end

  if castle_side == "q" then
    x_rook = 1
    f_rook_moved = did_queens_rook_move
  else
    x_rook = 8
    f_rook_moved = did_kings_rook_move
  end

  -- Check if king and room has ever moved
  if did_king_move(board, side) then 
    return false
  end

  if f_rook_moved(board, side) then
    return false
  end

  -- Check that the space between the king and rook are empty
  if not is_free_path_between_king_and_rook(board, x_rook, y) then
    return false
  end
  
  -- Check that the king does not in check and that the king would be moving
  -- through or into check if it were to castle
 
  return is_safe_to_castle(board, y, castle_side, side)

end

-- Note: Castling is a King move. The move is the new location for the king
-- after castling
function v_moves_castle(board, side) 

  local moves = {}
  local y
  local x_king = 5

  if side == "w" then
    y = 1
  else
    y = 8
  end

  if can_castle_king(board, side, "k") then
    table.insert(moves, {row=y, column=x_king + 2})
  end

  if can_castle_king(board, side, "q") then
    table.insert(moves, {row=y, column=x_king - 2})
  end

  return moves
end

-- This is like the move() function except made 
-- for castling
-- This function assumes the new_pos is valid
-- new_pos is the new position for the King 
-- (castling is King move)
function move_castle(board, new_pos, side)

  local x_rook
  local y_rook = new_pos.row

  if new_pos.column == 3 then
    x_rook = 1
  else
    x_rook = 8
  end

  move_king_rook(board, x_rook, y_rook)
end

-- x_rook, y_rook is the original starting position for the
-- rook that is castling (this identifies which rook is castling)
function move_king_rook(board, x_rook, y_rook) 

  local x_king = 5
  local new_x_rook
  local new_x_king

  if x_rook == 1 then
    new_x_king = 3
    new_x_rook = 4
  else
    new_x_king = 7
    new_x_rook = 6
  end

  -- Move the rook
  move(board, {row = y_rook, column = x_rook}, {row = y_rook, column = new_x_rook})
  -- Move the King
  move(board, {row = y_rook, column = x_king}, {row = y_rook, column = new_x_king})

  -- The move function will save the last move that in new_board.old_pos and new_board.new_pos,
  -- but we need to remember two moves for castling because we're moving two pieces at once.
  -- The move function will also clears board.extra_old_pos and board.extra_new_pos, so we must
  -- therefore set these values after we call move (not before)

  board.extra_old_pos = {row = y_rook, column = x_rook}
  board.extra_new_pos = {row = y_rook, column = new_x_rook}

end


-- This function is like the get_moves() function inside valid_moves.lua except this one
-- is specially for castling
function get_castle_moves(board, side)

  local board_moves = {}
  local y
  local x_king = 5

  if side == "w" then
    y = 1
  else
    y = 8
  end

  -- Castling on the king's side:
  if can_castle_king(board, side, "k") then
    
    local new_board = copy_board(board)
    local x_rook = 8

    move_king_rook(new_board, x_rook, y)
    table.insert(board_moves, new_board)
  end

  -- Castling on the queen's side:
  if can_castle_king(board, side, "q") then

    local new_board = copy_board(board)
    local x_rook = 1

    move_king_rook(new_board, x_rook, y)

    table.insert(board_moves, new_board)

  end
    

  return board_moves;
end

-- This function is like the is_valid_move() function found in valid_moves.lua,
-- excecpt this is specially made for castling.
function is_valid_move_castle(board, old_pos, new_pos, side) 

  -- Check if the piece is a King (castling is a King move)
  if not is_piece(king, board, side, old_pos) then
    return false
  end

  local moves = v_moves_castle(board, side)
  if #moves == 0 then
    return false
  end

  local i
  print_positions(moves)
  for i=1, #moves, 1 do

    if is_position_equal(moves[i], new_pos) then
      return true
    end
  end

  return false
end
