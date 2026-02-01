#include "Knight.h"
#include "../Board.h"
#include "../Move.h"

Knight::Knight(Color c, int r, int col) : Piece(PieceType::Knight, c, r, col) {}

std::vector<Move> Knight::getPseudoLegalMoves(const Board& board) const {
    std::vector<Move> moves;

    // L-shaped moves
    int offsets[8][2] = {
        {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
        {1, -2}, {1, 2}, {2, -1}, {2, 1}
    };

    for (const auto& offset : offsets) {
        int newRow = row + offset[0];
        int newCol = col + offset[1];

        if (!isValidSquare(newRow, newCol)) continue;

        if (board.isEmpty(newRow, newCol)) {
            moves.push_back(Move(row, col, newRow, newCol, MoveType::Normal));
        } else if (board.isEnemyPiece(newRow, newCol, color)) {
            moves.push_back(Move(row, col, newRow, newCol, MoveType::Capture));
        }
    }

    return moves;
}

Piece* Knight::clone() const {
    Knight* copy = new Knight(color, row, col);
    copy->hasMoved = hasMoved;
    return copy;
}
