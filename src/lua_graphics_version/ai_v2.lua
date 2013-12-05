
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
--dofile "chess.lua"

ai_v2 = {}

function ai_v2.init(side)

  ai_v2.max_contenders = 5 --  number of contenders to consider (a contender is a move, which is a board.)
  ai_v2.levels = 2 -- number of moves to look ahead

  ai_v2.side = side -- The side the ai is playing, either "w" or "b" (white or black)

end

function greater_than(score1, score2)
  return score1 > score2
end

function less_than(score1, score2)
  return score1 < score2
end


-- 
-- This function updates the list of 'containers' by either adding 'board' to 'contenders' or not
-- depending the result of calling the 'comp_f' function.
--
-- The size of 'contenders' is 'ai_v2.max_contenders'. 'board' will be added to 'contenders' if 
-- 1) the size of 'contenders' is less than 'ai_v2.max_contenders' or 2) calling comp_f(,)
-- returns true for at least one entry in contenders.
--
-- 'comp_f' is a function taking two scores. comp_f(score1, score2) returns true if score1 should be 
-- considered over score2.
--
-- For example, if comp_f = greater_than, then comp_f(100, 10) will return true.
--
--
function ai_v2.apply_candidate(contenders, board, score, side, comp_f)

  local candidate_move = { score = score, board = board }

  -- Check size
  if #contenders < ai_v2.max_contenders then
    table.insert(contenders, candidate_move)
    return
  end

  -- Look through contenders table. Replace the first entry you find where comp_f() returns true
  local i
  for i=1, #contenders, 1 do
    
    if comp_f(candidate_move.score, contenders[i].score) then

      contenders[i] = candidate_move
      return
    end
  end

end

-- Returns a board. This is the move which the ai makes in the game. 
function ai_v2.make_move(board)

  local is_our_turn = true
  local level = ai_v2.levels
  

  local contenders = ai_v2.get_candidate_moves(board, side, greater_than)
  -- local moves = get_moves_for_side(board, ai_v2.side)
  

  -- TODO: check/stale mate?

  local i
  for i=1, #contenders, 1 do
  
    contenders[i].score = ai_v2.recursive_score(contenders[i].board, ai_v2.levels, true)
    
  end
  
  local contender = ai_v2.get_top_contender(contenders, greater_than)

  return contender.board
end

function ai_v2.score(board)

  return get_score_ex(board, ai_v2.side)

end

function ai_v2.recursive_score(board, level, is_our_turn)

  -- Base case
  if level == 0 then
    return ai_v2.score(board)
  end

  local comp_f
  local side
  local contenders

  if is_our_turn then
    -- AI makes the move that's best for AI
    comp_f = greater_than
    side = ai_v2.side

  else
    -- The opponent makes the move that's worst for AI
    comp_f = less_than
    side = opponent_side(ai_v2.side)
  end

  -- Get candidate moves
  contenders = ai_v2.get_candidate_moves(board, side, comp_f)
  --ai_v2.print_candidate_moves(contenders)

  -- What about check mate or stale mates here?
  -- TODO: check for check/stale mate here

  local i
  for i=1, #contenders, 1 do
    -- Replace score with the recursive score
    contenders[i].score = ai_v2.recursive_score(contenders[i].board, level-1, not is_our_turn)
  end

  local contender = ai_v2.get_top_contender(contenders, comp_f)

  return contender.score
end

-- This will return a list containing top/bottom moves for consideration
-- depending on the comp_f() function
function ai_v2.get_candidate_moves(board, side, comp_f)

  local contenders = {}

  local board_moves = get_moves_for_side(board, side)
  local i

  if #board_moves == 0 then
    return contenders
  end

  for i=1, #board_moves, 1 do 
    ai_v2.apply_candidate(contenders, board_moves[i], ai_v2.score(board_moves[i]), side, comp_f)
  end

  return contenders
end

-- Print the moves saved in contenders
function ai_v2.print_candidate_moves(contenders)

  local i
  for i=1, #contenders, 1 do
    print_board(contenders[i].board)
    print("score:", contenders[i].score)
  end
end

-- Will return the top contender in contenders
function ai_v2.get_top_contender(contenders, comp_f)

  if #contenders == 0 then
    return nil
  end

  local max = contenders[1]

  if #contenders > 1 then

    local i
    for i=2, #contenders, 1 do

      if comp_f(contenders[i].score, max.score) then
	max = contenders[i]
      end
    end

    return max
  end
end

-- Testing

--[[
ai_v2.init("b")
test_board = set_board()
print_board(test_board)

test_board = ai_v2.make_move(test_board)
print_board(test_board)
]]
