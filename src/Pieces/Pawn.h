#pragma once

#include "../Piece.h"

class Pawn : public Piece {
public:
    Pawn(Color c, int r, int col);

    std::vector<Move> getPseudoLegalMoves(const Board& board) const override;
    Piece* clone() const override;

private:
    void addPromotionMoves(std::vector<Move>& moves, int fromRow, int fromCol,
                           int toRow, int toCol, bool isCapture) const;
};
