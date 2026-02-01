#pragma once

#include "../Piece.h"

class Rook : public Piece {
public:
    Rook(Color c, int r, int col);

    std::vector<Move> getPseudoLegalMoves(const Board& board) const override;
    Piece* clone() const override;
};
