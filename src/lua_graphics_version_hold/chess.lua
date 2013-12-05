
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

#!  /usr/bin/lua
dofile "util.lua";
dofile "eval.lua";
dofile "covers.lua";
dofile "valid_moves.lua";
dofile "input.lua";
--dofile "moves.lua";

print ("chess\n");

-- colours for displaying on terminal
-- vt100 codes:
disp_default = '\27[0m'; -- Reset
disp_white_side = '\27[37m'; -- 
disp_black_side = '\27[32m'; -- 

if not true then
  disp_default = ''; -- Reset
  disp_white_side = ''; -- 
  disp_black_side = ''; -- 
end

-- chess pieces
pawn = { moves_f = v_moves_pawn };
rook = { moves_f = v_moves_rook };
knight = { moves_f = v_moves_knight };
bishop = { moves_f = v_moves_bishop };
queen = { moves_f = v_moves_queen };
king = { moves_f = v_moves_king };

-- white chess pieces
white_pawn   = { piece = pawn,   ch = "p", side = "w", covers_f = covers_pawn_white };
white_rook   = { piece = rook,   ch = "r", side = "w", covers_f = covers_rook };
white_knight = { piece = knight, ch = "h", side = "w", covers_f = covers_knight }; 
white_bishop = { piece = bishop, ch = "b", side = "w", covers_f = covers_bishop };
white_queen  = { piece = queen,  ch = "q", side = "w", covers_f = covers_queen };
white_king   = { piece = king,   ch = "k", side = "w", covers_f = covers_king };

-- black chess pieces
black_pawn   = { piece = pawn,   ch = "P", side = "b", covers_f = covers_pawn_black };
black_rook   = { piece = rook,   ch = "R", side = "b", covers_f = covers_rook };
black_knight = { piece = knight, ch = "H", side = "b", covers_f = covers_knight };
black_bishop = { piece = bishop, ch = "B", side = "b", covers_f = covers_bishop };
black_queen  = { piece = queen,  ch = "Q", side = "b", covers_f = covers_queen };
black_king   = { piece = king,   ch = "K", side = "b", covers_f = covers_king };

-- Return an empty board
function empty_board()

  local board;

  board = {};
  for i=1, 8, 1 do
    board[i] = {};
  end

  return board;
end


-- Returns a board with all the pieces set to the starting positions
function set_board() 

  -- Start off with a new blank chess board
  local board = {};
  local i;
  for i=1, 8, 1 do
    board[i] = {};
  end;

  -- Set white side
  -- set the pawns
  for i=1, 8, 1 do
    board[2][i] =  { kind = white_pawn };
  end
  -- now the other pieces
  board[1][1] = { kind = white_rook, has_moved = false};
  board[1][2] = { kind = white_knight };
  board[1][3] = { kind = white_bishop };
  board[1][4] = { kind = white_queen };
  board[1][5] = { kind = white_king, has_moved = false};
  board[1][6] = { kind = white_bishop };
  board[1][7] = { kind = white_knight };
  board[1][8] = { kind = white_rook, has_moved = false};

  --Set black side

  -- set the pawns
  for i=1, 8, 1 do
    board[7][i] = { kind = black_pawn };
  end
  -- now the other pieces
  board[8][1] = { kind = black_rook, has_moved = false};
  board[8][2] = { kind = black_knight };
  board[8][3] = { kind = black_bishop };
  board[8][4] = { kind = black_queen };
  board[8][5] = { kind = black_king , has_moved = false};
  board[8][6] = { kind = black_bishop };
  board[8][7] = { kind = black_knight };
  board[8][8] = { kind = black_rook, has_moved = false};


  -- extra information
  board.old_pos = nil
  board.new_pos = nil


  return board;
end

function print_board(board, debug) 

  local i, j;

  print(disp_default);

  for i=8, 1, -1 do

    io.write (i);
    io.write (" ");
    for j=1, 8, 1 do

      if board[i][j] == nil then

	-- display " ." for white scuares
	-- display " +" for black squares

	if (i % 2 == 0 and j % 2 == 0) or (i % 2 ~= 0 and j % 2 ~= 0) then
	  io.write(" +");
	else
	  io.write(" .");
	end


      else
	if board[i][j].kind.side == "w" then
	  io.write(disp_white_side);
	else
	  io.write(disp_black_side);
	end

	io.write (" ");
	io.write(board[i][j].kind.ch);
	io.write(disp_default);
      end
    end
    print("");


  end 
  print ("\n   a b c d e f g h")
  print("\n");

  score_w, white_in_check, black_in_check, is_check_mate = score_white(board);

  --print("Score white: ", score_w);
  --print("Score black: ", score_black(board));

  print("White in check: ", white_in_check);
  print("Black in check: ", black_in_check);
  -- print("Check mate: ", is_check_mate); -- This isn't quite working


  --[[
  -- Just for testing
  if board.new_pos ~= nil then
  print (board[board.new_pos.row][board.new_pos.column].has_moved)
  end
  --]]
  --
  --

  debug = true
  if debug then
    return
  end
  print()
  print ("white king moved:", did_king_move(board, "w"))
  print ("black king moved:", did_king_move(board, "b"))

  print ()
  print ("white:")
  print ("kings rook moved", did_kings_rook_move( board, "w"))
  print ("queens rook moved", did_queens_rook_move(board,  "w"))

  print ()
  print ("black:")
  print ("kings rook moved", did_kings_rook_move(board, "b"))
  print ("queens rook moved", did_queens_rook_move(board, "b"))

  print ()
  print("white:")
  print("castle on king side", can_castle_king(board, "w", "k"))
  print("castle on queen side", can_castle_king(board, "w", "q"))

  print ()
  print("black:")
  print("castle on king side", can_castle_king(board, "b", "k"))
  print("castle on queen side", can_castle_king(board, "b", "q"))




end

-- This function moves a piece to anywhere. It doesn't check if is legal
-- Precondition: there is a piece located at the old_pos
-- This function clears board.extra_old_pos and board.extra_new_pos. 
-- board.extra_old_pos and board.extra_new_pos is set by move_king_rook() function
-- after call this move() function.
function move(board, old_pos, new_pos) 

  -- Copy the value at the old position to the new position
  board[new_pos.row][new_pos.column] = board[old_pos.row][old_pos.column]
  board[old_pos.row][old_pos.column] = nil;

  -- extra information --

  -- Save the move
  board.old_pos = old_pos
  board.new_pos = new_pos

  -- Castling
  -- if a castle or king is moving, we must make a note of it, this will determine
  -- whether castling is allowed in the future..for example, you can not castle once
  -- a king has previously moved. Also you can't move castle with a rook that has previously moved
  local piece = board[new_pos.row][new_pos.column].kind.piece

  if piece == king or piece == rook then
    board[new_pos.row][new_pos.column].has_moved = true;
  end

  -- Clear these values. These are used for castling. 
  -- In the event of castling, these to values will be set by move_king_rook() function
  board.extra_old_pos = nil
  board.extra_new_pos = nil

  return board;

end

function play_game_v1()
  board = set_board();
  print_board(board);

  while true do

    print ("\nEnter move:");
    old_pos, new_pos = get_user_move();
    while not is_on_board(old_pos) or not is_on_board(new_pos) do
      print ("invalid move");
      old_pos, new_pos = get_user_move();
    end

    move (board, old_pos, new_pos);
    print_board(board);


    -- ai
    board = get_best_board(get_moves_for_side(board, "b"), "b");
    if board ~= nil then
      print_board(board);
    else 
      print ("\nGame Over!");
      break;
    end

    -- check for stale/check mate
    tmp_board = copy_board(board);
    tmp_board = get_best_board(get_moves_for_side(board, "w"), "w");
    if tmp_board == nil then
      print ("\nGame Over!");
      break;
    end


  end
end

function play_game_v2() 

  local board = set_board();

  print_board(board);

  print ("\nscore for white: ", recursive_get_score(board, "w", "w", 2));

end


function play_game_v3()

  board = set_board();
  io.write('\27[2J\27[;H');
  print_board(board);

  while true do

    print ("\nEnter move:");
    old_pos, new_pos = get_user_move();
    while not is_on_board(old_pos) or not is_on_board(new_pos) do
      print ("invalid move");
      old_pos, new_pos = get_user_move();
    end

    move (board, old_pos, new_pos);
    io.write('\27[2J\27[;H');
    print_board(board);

    print ("\nthinking...");

    -- ai
    board = recursive_get_best_board(get_moves_for_side(board, "b"), "b", 2);
    if board ~= nil then

      -- chr(27) . "[2J" . chr(27) . "[;H";
      io.write('\27[2J\27[;H');


      print_board(board);
    else 
      print ("\nGame Over!");
      break;
    end

    -- check for stale/check mate
    tmp_board = copy_board(board);
    tmp_board = get_best_board(get_moves_for_side(board, "w"), "w");
    if tmp_board == nil then
      print ("\nGame Over!");
      break;
    end
  end
end

function play_game_v4()

  board = set_board();
  io.write('\27[2J\27[;H');
  print_board(board);

  while true do

    print ("\nEnter move:");
    old_pos, new_pos = get_user_move();
    --while not is_on_board(old_pos) or not is_on_board(new_pos) do
    while not is_valid_move(board, old_pos, new_pos, "w") do
      print ("invalid move");
      old_pos, new_pos = get_user_move();
    end

    move (board, old_pos, new_pos);
    io.write('\27[2J\27[;H');
    print_board(board);

    print ("\nthinking...");

    -- ai
    board = get_best_board_ex(get_moves_for_side(board, "b"), "b", 2);
    if board ~= nil then

      -- chr(27) . "[2J" . chr(27) . "[;H";
      io.write('\27[2J\27[;H');


      print_board(board);
    else 
      print ("\nGame Over!");
      break;
    end

    -- check for stale/check mate
    tmp_board = copy_board(board);
    tmp_board = get_best_board_ex(get_moves_for_side(board, "w"), "w");
    if tmp_board == nil then
      print ("\nGame Over!");
      break;
    end
  end
end

--play_game_v4();

--play_game_v1();
