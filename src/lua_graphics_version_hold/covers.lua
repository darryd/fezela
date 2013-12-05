
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

-- this is the helper function
-- it returns two variables up, and right.. 
-- posible values for up and right: -1, 0, 1
--
-- dir is one of n, ne, e, se, s, sw, w, nw
function get_up_right(dir)

  -- These values can be: -1, 0, 1
  local up = 0;
  local right = 0;

  if string.find(dir, "n", 1) ~= nil then
    up = 1;
  end
  
  if string.find(dir, "e", 1) ~=  nil then
    right = 1;
  end
  
  if string.find(dir, "s", 1) ~= nil then
    up = -1;
  end

  if string.find(dir, "w", 1) ~=  nil then
    right = -1
  end


  return up, right;

end

-- This returns all positions covered in this direction..(all blank squares up to and including
-- the last occupied position.) Does not include the position being occupied at "position".
-- This is helper function for covers_rook, covers_bishop, and covers_queen 
-- (but covers_queen = covers_rook + covers_bishop)
function covers_dir(board, dir, position)

  local covers = {};
  up, right = get_up_right(dir);

  pos = {};
  pos.row = position.row;
  pos.column = position.column;


  while true do

    pos.row = pos.row + up;
    pos.column = pos.column + right;

    -- Are we still on the board
    --if pos.row == 0 or pos.row == 9 or pos.column == 0 or pos.column == 9 then
    if not is_on_board(pos) then
      break;
    end;

    table.insert( covers, { row = pos.row, column = pos.column } );
 
    --row = pos.row;
    --column = pos.column;
    
    --print ("debug: inside function: row: ", pos.row, " column : ", pos.column);

    if board[pos.row][pos.column] ~= nil then --or board[pos.row][pos.column].kind ~= nil then
      -- A piece is here
      break;
    end

  end

  return covers;

end

-- Append table2 to table1
-- Returns a new table which is the table1 & table2 
-- Not efficient with time since it makes a copy..okay for v1 anyway
function merge_tables(table1, table2) 

  new_table = {};

  i = 1;

  -- Copy over table 1
  while i <= #table1 do
    new_table[i] = table1[i];
    i = i + 1;
  end

  -- Copy over table 2
  j = 1;
  while j <= #table2 do
    new_table[i] = table2[j];
    i = i + 1;
    j = j + 1;
  end

  return new_table;

end

function print_positions (positions) 

  for i=1, #positions do

    io.write('(', positions[i].row, ',', positions[i].column, ')' );

    i = i + 1;

  end

  print("");

end
 
function covers_rook(board, position) 

  local cover = covers_dir(board, "n", position);
  cover = merge_tables(cover, covers_dir(board, "e", position));
  cover = merge_tables(cover, covers_dir(board, "s", position));
  cover = merge_tables(cover, covers_dir(board, "w", position));

  return cover;
end

function covers_bishop(board, position)

  local cover = covers_dir(board, "ne", position);
  cover = merge_tables(cover, covers_dir(board, "se", position));
  cover = merge_tables(cover, covers_dir(board, "sw", position));
  cover = merge_tables(cover, covers_dir(board, "nw", position));

  return cover;
end

-- queen equals rook plus bishop
function covers_queen(board, position) 

  local cover = covers_rook(board, position);
  cover = merge_tables(cover, covers_bishop(board, position));

  return cover;
end

-- checks if a position is on the board
function is_on_board(position) 
  return position.row >= 1 and position.row <= 8 and position.column >= 1 and position.column <= 8;
end

function add_positions(position1, position2)

  sum = {};

  sum.row = position1.row + position2.row;
  sum.column = position1.column + position2.column;

  return sum;
end

function insert_if_on_board(covers, position) 

  if is_on_board(position) then
    table.insert(covers, position)
  end
end

function covers_king(board, position) 

  local covers = {};

  north = {row = 1, column = 0};
  east = {row = 0, column = 1};
  south = {row = -1, column = 0};
  west = { row = 0, column = -1};

  north_east = add_positions(north, east);
  south_east = add_positions(south, east);
  north_west = add_positions(north, west);
  south_west = add_positions(south, west);

  insert_if_on_board(covers, add_positions(north, position));
  insert_if_on_board(covers, add_positions(north_east, position));
  insert_if_on_board(covers, add_positions(east, position));
  insert_if_on_board(covers, add_positions(south_east, position));
  insert_if_on_board(covers, add_positions(south, position));
  insert_if_on_board(covers, add_positions(south_west, position));
  insert_if_on_board(covers, add_positions(west, position));
  insert_if_on_board(covers, add_positions(north_west, position));
  
  return covers;
end

function covers_pawn(board, position, side) 

  if side == "w" then
    direction = 1;
  else
    direction = -1;
  end

  local covers = {};

  left  = add_positions( { row = direction, column = -1 }, position);
  right = add_positions( { row = direction, column = 1  }, position);

  insert_if_on_board(covers, left);
  insert_if_on_board(covers, right);

  return covers;
end

function covers_pawn_white(board, position) 
  
  return covers_pawn(board, position, "w");
end

function covers_pawn_black(board, position)

  return covers_pawn(board, position, "b");
end

function covers_knight(board, position)

  covers = {};

  -- All possible moves a knight can make. 8 in total.
  -- When the row is 2 column can by -1 or 1
  -- and when the row is 1 the column can be -2 or 2
  insert_if_on_board(covers, add_positions( { row =  2, column =  1 }, position));
  insert_if_on_board(covers, add_positions( { row =  2, column = -1 }, position));
  insert_if_on_board(covers, add_positions( { row =  1, column =  2 }, position));
  insert_if_on_board(covers, add_positions( { row =  1, column = -2 }, position));
  insert_if_on_board(covers, add_positions( { row = -1, column =  2 }, position));
  insert_if_on_board(covers, add_positions( { row = -1, column = -2 }, position));
  insert_if_on_board(covers, add_positions( { row = -2, column =  1 }, position));
  insert_if_on_board(covers, add_positions( { row = -2, column = -1 }, position));
  
  return covers;

end
