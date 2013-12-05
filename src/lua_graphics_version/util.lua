
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

#! /usr/bin/lua

function opponent_side(side) 

  if side == "w" then
    return "b";
  else
    return "w";
  end
end

function is_on_board(position) 

  return position.row >= 1 and position.row <= 8 and position.column >= 1 and position.row <= 8;

end

function copy_position(position)

  if position == nil then
    return nil
  end

  return {row = position.row, column = position.column}
end

-- Returns a copy of a board
function copy_board(board)

  copy = {};

  for i=1, 8, 1 do

    copy[i] = {};
    for j=1, 8, 1 do

      if board[i][j] ~= nill then
	copy[i][j] = { kind = board[i][j].kind, has_moved = board[i][j].has_moved };
      end

    end
  end

  -- extra information
  copy.old_pos = copy_position(board.old_pos)
  copy.new_pos = copy_position(board.new_pos)
  copy.extra_old_pos = copy_position(board.extra_old_pos)
  copy.extra_new_pos = copy_position(board.extra_new_pos)

  return copy;
end

