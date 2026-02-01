#pragma once

#include "Board.h"
#include "Move.h"
#include <limits>

class AI {
private:
    int maxDepth;
    Color aiColor;

    // Piece-Square Tables for positional evaluation
    static const int pawnTable[8][8];
    static const int knightTable[8][8];
    static const int bishopTable[8][8];
    static const int rookTable[8][8];
    static const int queenTable[8][8];
    static const int kingMiddleGameTable[8][8];
    static const int kingEndGameTable[8][8];

    int evaluate(const Board& board) const;
    int getPieceSquareValue(const Piece* piece, bool isEndGame) const;
    bool isEndGame(const Board& board) const;

    int minimax(Board& board, int depth, int alpha, int beta, bool maximizing);
    void orderMoves(std::vector<Move>& moves, Board& board);

public:
    AI(Color color, int depth = 4);

    Move getBestMove(Board& board);
    void setDepth(int depth) { maxDepth = depth; }
    int getDepth() const { return maxDepth; }
};
