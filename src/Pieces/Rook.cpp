#include "Rook.h"
#include "../Board.h"
#include "../Move.h"

Rook::Rook(Color c, int r, int col) : Piece(PieceType::Rook, c, r, col) {}

std::vector<Move> Rook::getPseudoLegalMoves(const Board& board) const {
    std::vector<Move> moves;

    // 4 straight directions
    int directions[4][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
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

Piece* Rook::clone() const {
    Rook* copy = new Rook(color, row, col);
    copy->hasMoved = hasMoved;
    return copy;
}
