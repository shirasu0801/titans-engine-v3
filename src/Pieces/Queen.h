#pragma once

#include "../Piece.h"

class Queen : public Piece {
public:
    Queen(Color c, int r, int col);

    std::vector<Move> getPseudoLegalMoves(const Board& board) const override;
    Piece* clone() const override;
};
