
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

pawn_v = 10;
rook_v = 200;
knight_v = 53;
bishop_v = 53;
queen_v = 600;
king_v = 20;

--check_mate_v = 500;
check_mate_v = 50;

advancement_factor = 100.5;

function get_value(piece)

  if piece == pawn then
    return pawn_v;
  end

  if piece == rook then
    return rook_v;
  end

  if piece == knight then
    return knight_v;
  end

  if piece == bishop then
    return bishop_v;
  end

  if piece == queen then
    return queen_v;
  end

  if piece == king then
    return king_v;
  end

end

-- The closer you are to the centre, the higher the value
function get_base_value(position)

  if (position.row == 4 or position.row == 5) and (position.column == 4 or position.column == 5) then
    return 3;
  end

  if (position.row >= 3 and position.row <= 6) and (position.column >= 3 and position.column <= 6) then
    return 2;
  end

  return 1;

end

function print_base_values() 

  for i = 1, 8, 1 do
    for j=1, 8, 1 do
      position = {};
      position.row = i;
      position.column = j;
      value = get_base_value(position);

      io.write(value, " ");
    end
    print("");
  end

end

-- Returns score for points for pieces
-- Returns white_score, black_score
function add_up_pieces(board)

  local white_score = 0;
  local black_score = 0;

  local i
  local j

  for i=1, 8, 1 do
    for j=1, 8, 1 do
      if board[i][j] ~= nil then
	if board[i][j].kind.side == "w" then
	  white_score = white_score + get_value(board[i][j].kind.piece) * 100
	else
	  black_score = black_score + get_value(board[i][j].kind.piece) * 100
	end
      end
    end
  end

  return white_score, black_score;

end

-- Return score, is_in_check
function score_covering_position(position, board, side)

  local score = 0;
  local is_in_check = false;

  -- Firstly, you get points for the position you cover
  score = get_base_value(position);
  --io.write (">>debug: score for base_value: ", score, "\n");
  --io.write (">> row, column: ", position.row, " ", position.column, "\n");

  -- Secondly, you get points for your pieces getting closer into enemy territory
  if side == "w" then
    score = score + (position.row) * advancement_factor;
  else
    score = score + (8 - position.row + 1) * advancement_factor;
  end

  -- Thirdly, you get points if you're defending or threatening another piece
  if board[position.row][position.column] ~= nil then


    local piece_covered = board[position.row][position.column].kind.piece;
    local side_covered = board[position.row][position.column].kind.side;

    -- no points for protecting your own king, but threating the opponent's king, that's good
    if piece_covered ~= king then
      score = score + get_value(piece_covered) * 7; -- Experiment

      --io.write(">>debug: plus score covering a piece: ", get_value(piece_covered), "\n");

    end

    -- this means check
    if piece_covered == king and side ~= side_covered then

      is_in_check = true;
      score = score + get_value(king);

    end

  end

  return score, is_in_check;
end


--
function score_white(board) 

  local white_in_check = false;
  local black_in_check = false;
  local is_check_mate = false; -- Not yet working

  local white_score = 0;
  local black_score = 0;

  white_score, black_score = add_up_pieces(board);
  --io.write ("debug: white_score black_score: ", white_score, " ", black_score, "\n");



  -- Look through the entire board
  for i=1, 8, 1 do
    for j=1, 8, 1 do

      -- If there's a piece, check all the positions it covers
      if board[i][j] ~= nil and board[i][j].kind ~= nil then

	side = board[i][j].kind.side;
	pos = { row = i, column = j };

	--	print ("");
	--io.write ("debug: piece = ", board[i][j].kind.piece, "\n");
	--io.write ("debug: row =  ", i, " column = ", j, "\n");
	--	print ("");

	covers = board[i][j].kind.covers_f(board, pos);

	for k=1, #covers, 1 do

	  pos = covers[k];
	  score, is_in_check = score_covering_position( pos, board, side);
	  --io.write ("debug: score covering position: ", score, "\n");

	  -- Add score to total_scores
	  if side == "w" then
	    white_score = white_score + score;
	  else
	    black_score = black_score + score;
	  end

	  -- Check for if a king is in check.
	  if is_in_check then

	    if side == "w" then
	      black_in_check = true;
	    else
	      white_in_check = true;
	    end

	  end

	end


      end

    end
  end


  -- check for check mate
  --[[
  )if white_in_check and (get_best_board(get_moves_for_side(board, "w"), "w") == nil) then
    is_check_mate = true;
  end

  if black_in_check and (get_best_board(get_moves_for_side(board, "b"), "b") == nil) then
    is_check_mate = true;
  end
  --]]
  --TESTING
  --[[
  if board.extra_old_pos ~= nil then
    print "castling"
    black_score = black_score +  9999999
  else 
    -- black_score = 0
  end
]]


  return white_score - black_score, white_in_check, black_in_check, is_check_mate; -- check_mate isn't working
end

function score_black(board) 

  return - score_white(board);
end

function get_score(board, side)

--  print ("inside get_score: side = ", side);
  if side == "w" then
    return score_white(board);
  else
    return score_black(board);
  end

end

function get_average_score(boards, side)

  local average;
  local total = 0;
  local i

  if #boards > 0 then

    for i=1, #boards, 1 do
      total = total + get_score(boards[i], side);
    end  

    return total / #boards;
  end

  print ("debug: get_average_score(), no boards.");

end

function number_of_moves(board, side)

  board_moves = get_moves_for_side(board, side);
  
  return #board_moves;
end



function is_move_check_mate(board, side) 

  local is_in_check_mate = false;
  local in_check;

  s, white_in_check, black_in_check = get_score(board, side);
  if side == "w" then
    in_check = black_in_check;
  else
    in_check = white_in_check;
  end

  if in_check then
    
    tmp_board = copy_board(board);

    if number_of_moves(board, opponent_side(side)) == 0 then
      is_in_check_mate = true;
    end
    
  end

  return is_in_check_mate;
end
    
function get_min(numbers) 

  local minimum;

  minimum = numbers[1];

  if #numbers > 1 then

    for i=2, #numbers, 1 do

      if numbers[i] < minimum then
	minimum = numbers[i];
      end
    end 

    return minimum;
  end
end




--[[

This function gets the score than considers the moves the oppenent
will make, etc..

board - chess board
side - 
level is the number of moves to look ahead, level = 1 means don't look ahead

score = score(current_move) + average(possible counter moves)
--]]
function recursive_get_score(board, side, turn, level)

  -- Base case:
  if level == 0 then
    return 0;
  end

  local score;

  -- Score for this move 
  score = get_score(board, side);

  if is_move_check_mate(board, turn) then

    if turn == side then
      score = score + check_mate_v;
    else
      score = score - check_mate_v;
    end

    return score;
  end

  -- Let's avoid stale mate:
  if number_of_moves(board, opponent_side(side)) == 0 then
    return -check_mate_v;
  end

  -- Now look ahead...
  local temp_board = copy_board(board);
  level = level - 1;
  turn = opponent_side(turn);

  board_moves = get_moves_for_side(temp_board, turn);

  -- Find the average score of all the moves
  local total_score = 0;
  local average = 0;

  for i=1, #board_moves, 1 do
    total_score = total_score + recursive_get_score(temp_board, side, turn, level);
  end

  -- We already checked that #board_moves is not zero
  average = total_score / #board_moves;

  return score + average; -- Plus average of 
end

-- checks for check mate and stale mate
function get_score_ex(board, side)
  
  -- First check for check mate and stale mate
  if is_move_check_mate(board, "w") then
    
    return check_mate_v;

  else
    if number_of_moves(board, "b") == 0 then
      -- Let's avoid stale mate:
      return -check_mate_v;
    end
  end

  return get_score(board, side);
end

-- a hack..To replaced by something better..
function get_score_min_hack(board)

  local i

  -- Simply return the white's lowest posible score after any 
  -- of black's counter moves
  
  white_board = copy_board(board);

  if number_of_moves(white_board, "w") == 0 then
    return get_score_ex(white_board, "b"); -- 
  end

  -- Otherwise, look for white's best move (which results in black having
  -- the minimum score).
  white_board_moves = get_moves_for_side(white_board, "w");

  local scores = {}; -- This is scores after each of black's counter moves.
  
  for i=1, #white_board_moves, 1 do

    table.insert(scores, get_score_ex(white_board_moves[i]));
  end

  return get_min(scores);
end
