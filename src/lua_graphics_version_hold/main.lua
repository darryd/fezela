
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

-- Graphics written by Christopher Porter and Darry Danzig

dofile "game.lua"

function love.load()


  orientation = "w"
  initial_countdown = 5

  drag = false
  iDrag = -1
  rClick = 0
  moving = false
  targetX = 0
  targetY = 0

  w_pawn = love.graphics.newImage("w_pawn.png")
  w_rook = love.graphics.newImage("w_rook.png")
  w_knight = love.graphics.newImage("w_knight.png")
  w_bishop = love.graphics.newImage("w_bishop.png")
  w_queen = love.graphics.newImage("w_queen.png")
  w_king = love.graphics.newImage("w_king.png")

  b_pawn = love.graphics.newImage("b_pawn.png")
  b_rook = love.graphics.newImage("b_rook.png")
  b_knight = love.graphics.newImage("b_knight.png")
  b_bishop = love.graphics.newImage("b_bishop.png")
  b_queen = love.graphics.newImage("b_queen.png")
  b_king = love.graphics.newImage("b_king.png")

  moving_pieces = {}

  notify_turn = {turn = "player", countdown = initial_countdown}

  g_board = {}

  for i = 1, 8 do
    g_board[i] = {}

    for j = 1, 8 do
      if j % 2 == i % 2 then
	g_board[i][j] = 0
      else
	g_board[i][j] = 1
      end
    end
  end

  pieces = {}

  for i=1, 8, 1 do
    table.insert(pieces, {type=w_pawn, x=i-1, y=6, drag=false, is_alive=true});
  end

  table.insert(pieces, {type=w_rook, x=0, y=7, drag=false, is_alive=true});
  table.insert(pieces, {type=w_knight, x=1, y=7, drag=false, is_alive=true});
  table.insert(pieces, {type=w_bishop, x=2, y=7, drag=false, is_alive=true});
  table.insert(pieces, {type=w_queen, x=3, y=7, drag=false, is_alive=true});
  table.insert(pieces, {type=w_king, x=4, y=7, drag=false, is_alive=true});
  table.insert(pieces, {type=w_bishop, x=5, y=7, drag=false, is_alive=true});
  table.insert(pieces, {type=w_knight, x=6, y=7, drag=false, is_alive=true});
  table.insert(pieces, {type=w_rook, x=7, y=7, drag=false, is_alive=true});

  black = {}

  for i=1, 8, 1 do
    table.insert(pieces, {type=b_pawn,x=i-1, y=1, drag=false, is_alive=true});
  end

  table.insert(pieces, {type=b_rook, x=0, y=0, drag=false, is_alive=true});
  table.insert(pieces, {type=b_knight, x=1, y=0, drag=false, is_alive=true});
  table.insert(pieces, {type=b_bishop, x=2, y=0, drag=false, is_alive=true});
  table.insert(pieces, {type=b_queen, x=3, y=0, drag=false, is_alive=true});
  table.insert(pieces, {type=b_king, x=4, y=0, drag=false, is_alive=true});
  table.insert(pieces, {type=b_bishop, x=5, y=0, drag=false, is_alive=true});
  table.insert(pieces, {type=b_knight, x=6, y=0, drag=false, is_alive=true});
  table.insert(pieces, {type=b_rook, x=7, y=0, drag=false, is_alive=true});

  game.start()
end

-- isBoardCoords is true if g_board positions..
-- is false if screen coordinates
function clear_spot(pieces, x, y, isBoardCoords)

  local i

  for i=1, #pieces, 1 do

    if (not isBoardCoords and isClickedOn( x, y, pieces[i].x, pieces[i].y))
      or (isBoardCoords and pieces[i].x == x and pieces[i].y == y)
      
      then

      -- move off g_board
      pieces[i].x = -1;
      pieces[i].y = -1;

      pieces[i].is_alive = false;

      break;

    end

  end

end

function love.mousepressed( x, y, button )

  -- While pieces are moving, ignore mouse clicks
  if moving or #moving_pieces ~= 0 then
    return
  end

  if button == "l" then 
    
    for i = 1, #pieces do
      if isClickedOn( x, y, pieces[i].x, pieces[i].y ) then
	pieces[i].drag = true

	-- In case the move turns out to be invalid, lets remember the starting x and y
	pieces[i].original_x = pieces[i].x
	pieces[i].original_y = pieces[i].y

	iDrag = i
	return
      end
    end
  else 
    if rClick == 0 then
      for i = 1, #pieces do
	if isClickedOn( x, y, pieces[i].x, pieces[i].y ) then
	  rClick = i
	  return
	end
      end
    else
      if isValidMove( math.floor(x / 64), math.floor(y/64), pieces[rClick].x, pieces[rClick].y ) then
	moving = true
	targetX = math.floor(x / 64)
	targetY = math.floor(y / 64)
	clear_spot(pieces, x, y);
      else
	rClick = 0
      end

    end
  end
end

function love.mousereleased( x, y, button )
  -- Attempt to move
  local target_x = math.floor(x / 64)
  local target_y = math.floor(y / 64)


  if iDrag == -1 then
    return
  end


  if isValidMove( target_x, target_y, pieces[iDrag].x, pieces[iDrag].y ) then
    clear_spot(pieces, x, y);
    pieces[iDrag].x = target_x
    pieces[iDrag].y = target_y
    pieces[iDrag].drag = false
    -- notify ai..
    --game.your_turn()

    notify_ai_turn()

  else

    pieces[iDrag].x = pieces[iDrag].original_x
    pieces[iDrag].y = pieces[iDrag].original_y
    pieces[iDrag].drag = false
  end
  


  iDrag = -1
end


function draw_red_moving_pieces(moving_pieces)

  local i

  if moving_pieces == nil then
    return
  end

  for i=1, #moving_pieces, 1 do

    if moving_pieces[i].moving then

      love.graphics.setColor(90, 0, 0)
      love.graphics.rectangle( "fill", moving_pieces[i].targetX * 64, moving_pieces[i].targetY * 64, 64, 64 )
      
    end
  end

end



function love.draw()

  local i
  local j

  for i = 1, 8 do
    for j = 1, 8 do
      if( g_board[i][j] == 0 ) then
	love.graphics.setColor(255, 255, 255)
      else
	love.graphics.setColor(200, 200, 200)
      end


      love.graphics.rectangle( "fill", (i - 1) * 64, (j - 1) * 64, 64, 64 )
    end
  end


  draw_red_moving_pieces(moving_pieces)


  if rClick ~= 0 then
    if moving == false then

      love.graphics.setColor(90, 0, 0)
      love.graphics.rectangle( "fill", pieces[rClick].x * 64, pieces[rClick].y * 64, 64, 64 )

    else
      love.graphics.setColor(90, 0, 0)
      love.graphics.rectangle( "fill", targetX * 64, targetY * 64, 64, 64 )
    end
  end

  love.graphics.setColor(255, 255, 255)


  for i = 1, #pieces do 
    if pieces[i].drag == false then 

      if pieces[i].is_alive then


	love.graphics.draw(pieces[i].type, pieces[i].x * 64, pieces[i].y * 64)
      end
    else 
      local x, y = love.mouse.getPosition()
      x = x - 32
      y = y - 32

      love.graphics.draw(pieces[i].type, x, y)
    end
  end
end

function update_moving_piece(moving_piece, dt) 

  if moving_piece.moving then


    -- Check countdown first.. only when countdown == 0 then the piece can move

    if moving_piece.countdown > 0 then
      moving_piece.countdown = moving_piece.countdown - 1
      return
    end
      
    local x = 0
    local y = 0

    if moving_piece.targetX > pieces[moving_piece.i].x then
      x = 1
    elseif moving_piece.targetX < pieces[moving_piece.i].x then
      x = -1
    end

    if moving_piece.targetY > pieces[moving_piece.i].y then
      y = 1
    elseif moving_piece.targetY < pieces[moving_piece.i].y then
      y = -1
    end

    pieces[moving_piece.i].x = pieces[moving_piece.i].x + dt * 5 * x
    pieces[moving_piece.i].y = pieces[moving_piece.i].y + dt * 5 * y


    if math.abs( pieces[moving_piece.i].x - moving_piece.targetX) < .1 then
      pieces[moving_piece.i].x = moving_piece.targetX
    end

    if math.abs(pieces[moving_piece.i].y- moving_piece.targetY) < .1 then			
      pieces[moving_piece.i].y = moving_piece.targetY

    end

    if pieces[moving_piece.i].x == moving_piece.targetX and pieces[moving_piece.i].y == moving_piece.targetY then
      moving_piece.moving = false
    end
  end
end

function clean_moving_list(moving_pieces) 

  local before, after

  local indexes = {}
  local i


  before = #moving_pieces

  -- first find indexes to remove than remove them
  for i=#moving_pieces, 1, -1 do
    if not moving_pieces[i].moving then
      table.insert(indexes, i)
    end
  end

  -- remove indexes starting at the end first
  for i=1, #indexes, 1 do
    table.remove(moving_pieces, indexes[i])
  end

  after = #moving_pieces
  
  -- if the list wasn't empty when this function began
  -- but now it is, then that means ai's pieces have finished moving
  -- TODO do something like a signal
  
end

-- move pieces in the list ..and remove them from the list
-- when they reach their target..
function update_moving_pieces(moving_pieces, dt) 

  local i

  -- update_moving_piece(1,1)
  if moving_pieces == nil then
    return
  end

  for i=1, #moving_pieces, 1 do
    update_moving_piece(moving_pieces[i], dt)
  end

  clean_moving_list(moving_pieces);

end

function love.update( dt )

  update_notify_ai()
  update_moving_pieces(moving_pieces, dt)

  if moving then

    local x = 0
    local y = 0

    if targetX > pieces[rClick].x then
      x = 1
    elseif targetX < pieces[rClick].x then
      x = -1
    end

    if targetY > pieces[rClick].y then
      y = 1
    elseif targetY < pieces[rClick].y then
      y = -1
    end

    pieces[rClick].x = pieces[rClick].x + dt * 5 * x
    pieces[rClick].y = pieces[rClick].y + dt * 5 * y


    if math.abs( pieces[rClick].x - targetX) < .1 then
      pieces[rClick].x = targetX
    end

    if math.abs(pieces[rClick].y- targetY) < .1 then			
      pieces[rClick].y = targetY

    end

    if pieces[rClick].x == targetX and pieces[rClick].y == targetY then
      moving = false
      rClick = 0
      -- notify ai..
      --game.your_turn()
      notify_ai_turn()
    end

  end
end

function isClickedOn( x1, y1, x2, y2 )
  if x1 >= x2 * 64 and y1 >= y2 * 64 and x1 <= x2 * 64 + 64 and y1 <= y2 * 64 + 64 then
    return true
  end
  return false
end

function isValidMove( x2, y2, x1, y1 )

  local old_pos
  local new_pos

  old_pos = get_ai_choords(x1, y1, "w")
  new_pos = get_ai_choords(x2, y2, "w")

  return game.make_move(old_pos, new_pos)

end

-- Return -1 if no piece, otherwise return index
function graphics_get_piece(pieces, x, y)

  for i=1, #pieces, 1 do
    if pieces[i].x == x and pieces[i].y == y then
      return i
    end
  end

  return -1
end


function graphics_move(pieces, x1, y1, x2, y2) 


  local index = graphics_get_piece(pieces, x1, y1)
  
  if index == -1 then
    return
  end

  table.insert(moving_pieces, {i=index, targetX=x2, targetY=y2, moving=true, countdown=initial_countdown})
  clear_spot(pieces, x2, y2, true)
  
end


function get_graphics_chords(position, orientation) 

  local x
  local y

  if orientation == "w" then
    x = position.column - 1
    y = 8 - position.row
  else
    x = position.column - 1
    y = position.row - 1
  end 

  return x, y
end

function get_ai_choords(x, y, orientation)

  local position = {}

  if orientation == "w" then
    position.column = x + 1
    position.row = 8 - y
  else
    position.column = x + 1
    position.row = y + 1
  end

  return position

end

function notify_ai_turn() 

  notify_turn.turn = "ai"
  countdown = initial_countdown

end

function update_notify_ai()

  if notify_turn.turn == "ai" and notify_turn.countdown == 0 then
    
    notify_turn.turn = "player"
    notify_turn.countdown = initial_countdown
    game.your_turn()
    return
  end

  if notify_turn.turn == "ai" then
    notify_turn.countdown = notify_turn.countdown - 1
  end
end
