
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

-- example str is 'a5'
function str_to_pos(str)

  r = string.gfind(str, "%d");
  col = string.gfind(str, "%a");


  row = string.byte(r()) - string.byte('1') + 1;
  column = string.byte(col()) - string.byte('a') + 1;

  return { row = row, column = column };

end

-- Get move from user
function get_user_move()

  input = io.read();
 
  while not string.find(input, "^%s*%a%d%s+%a%d%s*$") do

    --[[
    if string.find("quit") then
      break;
    end
    --]]

    print ("Enter move as \"[column][row] [column][row]\"");
    print ("eg) \"a2 a4");
    input = io.read();

  end

  pos_str = string.gfind(input, "%a%d");

  old_pos = str_to_pos(pos_str());
  new_pos = str_to_pos(pos_str());

  return old_pos, new_pos;
end

