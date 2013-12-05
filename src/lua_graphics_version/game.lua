
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

dofile "chess.lua"
dofile "ai_v2.lua"


--local sched = require "sched"
--local mutex = require "mutex"

-- Assume player is white..ai is black

game = {}

function game.start()


  print ("hi")

  game.player = {}
  game.ai = {}
  game.turn = "w"
  game.board = set_board()

  ai_v2.init("b")

  io.write('\27[2J\27[;H');
  print_board(game.board)

end

-- Called when a player tries to make a move
-- returns true if move is valid
-- false otherwise
function game.make_move(old_pos, new_pos)


  if game.turn == "b" then
    return false
  end


  local is_valid =  is_valid_move(game.board, old_pos, new_pos, "w")
 
  if is_valid then
    game.turn = "b"
    move(game.board, old_pos, new_pos)
    io.write('\27[2J\27[;H');
    print_board(game.board)

    print ("ai's turn..")
  end

  if is_valid then
    return true
  end

  -- Castling ?
  is_valid = is_valid_move_castle(game.board, old_pos, new_pos, "w")
  
  if is_valid then

    -- Graphics will move the king..but the rook will need to move too
    -- The positions for the moving rook..will be in game.board.extra_old_pos and 
    -- game.board.extra_new_pos.
    game.turn = "b"

    move_castle(game.board, new_pos, "w")


    local x1, y1, x2, y2
    
    local x1, y1, x2, y2

    x1, y1 = get_graphics_chords(game.board.extra_old_pos, "w")
    x2, y2 = get_graphics_chords(game.board.extra_new_pos, "w")

    graphics_move(pieces, x1, y1, x2, y2)
    

  end

  return is_valid;
end

function love.keypressed(key)   -- we do not need the unicode, so we can leave it out
  --game.your_turn()
end

function game.your_turn()


  if game.turn == "w" then

    return
  end


  local tmp_board = get_best_board_ex(get_moves_for_side(game.board, "b"), "b", 2);
  
  --local tmp_board = ai_v2.make_move(game.board)
 
  if tmp_board == nil then
    return
  end

  game.board = tmp_board

  io.write('\27[2J\27[;H');
  print_board(game.board)
  game.turn = "w"

  local x1, y2, x2, y2


  x1, y1 = get_graphics_chords(game.board.old_pos, "w")
  x2, y2 = get_graphics_chords(game.board.new_pos, "w")

  
  graphics_move(pieces, x1, y1, x2, y2)

  -- Castling ?
  if game.board.extra_old_pos ~= nil then


    local x1, y1, x2, y2

    x1, y1 = get_graphics_chords(game.board.extra_old_pos, "w")
    x2, y2 = get_graphics_chords(game.board.extra_new_pos, "w")

    graphics_move(pieces, x1, y1, x2, y2)
  end

  print ("your turn..")


end
