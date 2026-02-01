#include "Queen.h"
#include "../Board.h"
#include "../Move.h"

Queen::Queen(Color c, int r, int col) : Piece(PieceType::Queen, c, r, col) {}

std::vector<Move> Queen::getPseudoLegalMoves(const Board& board) const {
    std::vector<Move> moves;

    // All 8 directions (combines rook and bishop)
    int directions[8][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},  // Rook-like
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}  // Bishop-like
    };

    for (const auto& dir : directions) {
        for (int i = 1; i < 8; ++i) {
            int newRow = row + dir[0] * i;
            int newCol = col + dir[1] * i;

            if (!isValidSquare(newRow, newCol)) break;

            if (board.isEmpty(newRow, newCol)) {
                moves.push_back(Move(row, col, newRow, newCol, MoveType::Normal));
            } else if (board.isEnemyPiece(newRow, newCol, color)) {
                moves.push_back(Move(row, col, newRow, newCol, MoveType::Capture));
                break;
            } else {
                break;  // Blocked by friendly piece
            }
        }
    }

    return moves;
}

Piece* Queen::clone() const {
    Queen* copy = new Queen(color, row, col);
    copy->hasMoved = hasMoved;
    return copy;
}
