#pragma once

#include "../Piece.h"

class King : public Piece {
public:
    King(Color c, int r, int col);

    std::vector<Move> getPseudoLegalMoves(const Board& board) const override;
    Piece* clone() const override;
};
