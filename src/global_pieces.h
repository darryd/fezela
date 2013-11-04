
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

// These pieces are global to save memory. 

    Rook w_rook;
    Rook w_k_rook(white, kings_rook);
    Rook w_q_rook(white, queens_rook);

    Rook b_rook(black);
    Rook b_k_rook(black, kings_rook);
    Rook b_q_rook(black, queens_rook);

    Knight w_knight;
    Knight b_knight(black);

    Bishop w_bishop;
    Bishop b_bishop(black);

    Queen w_queen;
    Queen b_queen(black);

    King w_king;
    King b_king(black);

    Pawn w_pawn;
    Pawn b_pawn(black);
