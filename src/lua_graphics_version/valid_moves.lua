
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

#!/usr/bin/lua

dofile "castling.lua"

-- Returns bool, side
function is_occupied(board, position)

  if board[position.row][position.column] == nil then

    return false, nil;
  end

  return true, board[position.row][position.column].kind.side;

end

function print_occupation(board) 

  local something_here
  local side

  for i=8, 1, -1 do
    for j=1, 8, 1 do
      something_here, side = is_occupied(board, {row = i, column=j});
      if something_here then
	io.write(side, " ");
      else
	io.write(". ");
      end
    end
    print ("");
  end
end

-- This function removes moves from the "moves" table that would be attacting its own pieces.
function v_dont_attack_self (moves, board, position, side)

  delete_indexes = {};

  -- find the indexes to removes from the "moves" table
  -- go backwards so that we can remove the index from the back
  -- since removing from the back will not affect any other indexes
  -- that appear before it.
  for i=#moves, 1, -1 do

    pos = moves[i];
    something_there, side_there = is_occupied(board, pos);

    if something_there and side == side_there then
      -- We cannot attack one of our own pieces
      table.insert(delete_indexes, i);
    end
  end

  -- Now remove indexes
  for i=1, #delete_indexes, 1 do
    table.remove(moves, delete_indexes[i]);
  end
end

function v_dont_attack_self_Ex(covers_f, board, position, side)

  local moves = covers_f(board, position);
  v_dont_attack_self(moves, board, position, side);

  return moves;
end

-- Valid rook moves. (Doesn't check if king is in check)
function v_moves_rook(board, position, side)

  return v_dont_attack_self_Ex(covers_rook, board, position, side);
end

-- Valid knight moves. (Doesn't check if king is in check)
function v_moves_knight(board, position, side)

  return v_dont_attack_self_Ex(covers_knight, board, position, side);
end

-- Valid bishop moves. (Doesn't check if king is in check)
function v_moves_bishop(board, position, side)

  return v_dont_attack_self_Ex(covers_bishop, board, position, side);
end

-- Valid queen moves. (Doesn't check if king is in check)
function v_moves_queen(board, position, side)

  return v_dont_attack_self_Ex(covers_queen, board, position, side);
end

-- Valid king moves. (Doesn't check if king is in check)
function v_moves_king(board, position, side)

  return v_dont_attack_self_Ex(covers_king, board, position, side);
end

-- TODO do en passant rule
function v_moves_pawn(board, position, side)

  moves = {};
  covers = covers_pawn(board, position, side);
  local direction;
  local starting_row;

  if side == "w" then
    direction = 1;
    starting_row = 2;
  else
    direction = -1;
    starting_row = 7;
  end

  --Check covered locations..A pawn can only move there if occupied by the other side
  for i=1, #covers, 1 do

    pos = covers[i];
    something_there, side_there = is_occupied(board, pos);

    if something_there and side_there ~= side then
      -- There's a piece belonging to the other side.
      table.insert(moves, pos);
    end
  end

  -- Check if it is possible to advance
  -- Can we move forward by one
  potential_pos = { row = position.row + direction, column = position.column };

  if is_on_board(potential_pos) then

    if not is_occupied(board, potential_pos) then
      table.insert(moves, potential_pos);

      -- Check if we can move forward by 2 
      if position.row == starting_row then

	potential_pos = { row = direction * 2 + starting_row, column = position.column };
	if not is_occupied(board, potential_pos) then
	  table.insert(moves, potential_pos);
	end
      end
    end
  end

  return moves;
end


-- This function returns a table of boards for each valid move of a piece. ("moves_f" variable is 
-- function appropriate for the piece. eg) v_moves_rook(), v_moves_pawn(), etc.
--
-- Function checks that the player isn't putting (or leaving) themselves in check. If a move
-- puts (or leaves) a player in check, that move will not be included in the table
-- 
-- For castling see get_castle_moves() inside of castling.lua source file
function get_moves(board, position, side, moves_f)

  local board_moves = {}; -- This will be the boards for each move the peice at "position" can make

  local moves = moves_f(board, position, side);


  -- For each move, add it to "board_moves" if the player isn't in check
  for i=1, #moves, 1 do

    new_pos = moves[i];
    new_board = copy_board(board);

    move(new_board, position, new_pos);
    score, white_in_check, black_in_check = score_white(new_board);

    if (side == "w" and not white_in_check) or (side == "b" and not black_in_check) then
      table.insert(board_moves, new_board);
    end
  end

  return board_moves;
end

function is_position_equal(pos1, pos2)

  if pos1.row ~= pos2.row then
    return false;
  end

  if pos1.column ~= pos2.column then
    return false;
  end

  return true;
end

-- This function is to check if a move is valid
-- For castling see is_valid_move_castle() found in castling.lua
function is_valid_move(board, old_pos, new_pos, side)

  -- If "old_pos" or new_pos isn't even on the board, then it is not valid
  if not is_on_board(old_pos) or not is_on_board(new_pos) then
    return false;
  end

  -- If there's no piece there, then it is not valid
  something_there, side_there = is_occupied(board, old_pos);
  if not something_there then
    return false;
  end

  -- The move is not valid if the piece doesn't belong to "side"
  if side ~= side_there then
    return false;
  end

  -- If the piece isn't in the array returned by moves_f, then it's not valid
  local moves = board[old_pos.row][old_pos.column].kind.piece.moves_f(board, old_pos, side);

  if #moves == 0 then
    return false;
  end

  local is_found = false;
  for i=1, #moves, 1 do
   
    local valid_move = moves[i];

    if is_position_equal(valid_move, new_pos) then
      is_found = true;
      break;
    end
  end

  if not is_found then
    return false;
  end

  -- If the move, puts or leaves the "side" in check, it's not valid
  local new_board = copy_board(board);

  move(new_board, old_pos, new_pos);
  score, white_in_check, black_in_check = score_white(new_board);

  if (side == "w" and white_in_check) or (side == "b" and black_in_check) then
   return false; 
  end

  -- Passed all the tests
  return true;
end

function print_boards(boards)

  for i=1, #boards, 1 do
    print_board(boards[i]);
  end
end

function get_moves_for_side(board, side)

  board_moves = {};

  for i=1, 8, 1 do
    for j=1, 8, 1 do

      something_there, side_there = is_occupied(board, {row=i,column=j});
      if something_there and side_there == side then

	moves_f = board[i][j].kind.piece.moves_f;

	board_moves = merge_tables(board_moves, get_moves(board, {row=i,column=j}, side, moves_f));

	-- Now if the piece is a King, it might be posible to castle
	

	-- TESTING !!!!
	-- Castle if you can..
	--junk = get_castle_moves(board, side)
	--if #junk ~= 0 then
	--  board_moves = get_castle_moves(board, side)

	--end

	board_moves = merge_tables(board_moves, get_castle_moves(board, side))

      end
    end
  end

  return board_moves;
end

-- TODO This should be in eval or ai
function get_best_board(boards, side)


  -- print ("inside get_best_board(): side = ", side);

  local max_board;
  local max_score;

  if #boards >= 1 then
    max_board = boards[1];
    max_score = get_score(max_board, side);
  end

  -- print_board(boards[1]);
  --print ("new_score: ", max_score);
  --print ("max_score: ", max_score);
  --print ("");

  for i=2, #boards, 1 do

    --    print_board(boards[1]);
    new_score = get_score(boards[i], side);

    if (new_score > max_score) then
      max_board = boards[i];
      max_score = new_score;
    end

    --print ("new_score: ", new_score);
    --print ("max_score: ", max_score);
    --print ("");

  end

  return max_board;
end

-- TODO This should be in eval or ai
function recursive_get_best_board(boards, side, level)


  -- print ("inside get_best_board(): side = ", side);

  local max_board;
  local max_score;

  if #boards >= 1 then
    max_board = boards[1];
    --max_score = get_score(max_board, side);
    max_score = recursive_get_score(board, side, side, level);
  end

  -- print_board(boards[1]);
  --print ("new_score: ", max_score);
  --print ("max_score: ", max_score);
  --print ("");

  for i=2, #boards, 1 do

    --    print_board(boards[1]);
    --new_score = get_score(boards[i], side);
    new_score = get_score(boards[i], side, side, level);

    if (new_score > max_score) then
      max_board = boards[i];
      max_score = new_score;
    end

    --print ("new_score: ", new_score);
    --print ("max_score: ", max_score);
    --print ("");

  end

  return max_board;
end

-- TODO This should be in eval or ai
-- Same as get_best_board except it uses get_score_ex instead of get_score
function get_best_board_ex(boards, side)


  -- print ("inside get_best_board(): side = ", side);

  local max_board;
  local max_score;
  local i

  if #boards >= 1 then
    max_board = boards[1];
    --max_score = get_score(max_board, side);
    max_score = get_score_ex(max_board, side);
  end

  -- print_board(boards[1]);
  --print ("new_score: ", max_score);
  --print ("max_score: ", max_score);
  --print ("");

  for i=2, #boards, 1 do

    --    print_board(boards[1]);
    --new_score = get_score(boards[i], side);
    new_score = get_score_ex(boards[i], side);

    if (new_score > max_score) then
      max_board = boards[i];
      max_score = new_score;
    end

    --print ("new_score: ", new_score);
    --print ("max_score: ", max_score);
    --print ("");

  end

  return max_board;
end
