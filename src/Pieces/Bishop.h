#pragma once

#include "../Piece.h"

class Bishop : public Piece {
public:
    Bishop(Color c, int r, int col);

    std::vector<Move> getPseudoLegalMoves(const Board& board) const override;
    Piece* clone() const override;
};
