#pragma once

#include "../Piece.h"

class Knight : public Piece {
public:
    Knight(Color c, int r, int col);

    std::vector<Move> getPseudoLegalMoves(const Board& board) const override;
    Piece* clone() const override;
};
