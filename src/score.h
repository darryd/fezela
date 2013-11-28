
/*

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

*/
#pragma once

class ScoreKeeper {
  public:
    virtual int get_score(const Board &board, Side side, bool is_our_turn) = 0;
    virtual ~ScoreKeeper(){};
};

class AlphaScore : public ScoreKeeper {
  public:
    AlphaScore();
    virtual ~AlphaScore(){};
    virtual int get_score(const Board &board, Side side, bool is_our_turn);
  private:
    std::map<Kind, int > values;
};
