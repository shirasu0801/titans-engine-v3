#include "King.h"
#include "../Board.h"
#include "../Move.h"

King::King(Color c, int r, int col) : Piece(PieceType::King, c, r, col) {}

std::vector<Move> King::getPseudoLegalMoves(const Board& board) const {
    std::vector<Move> moves;

    // Normal king moves (one square in any direction)
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) continue;

            int newRow = row + dr;
            int newCol = col + dc;

            if (!isValidSquare(newRow, newCol)) continue;

            if (board.isEmpty(newRow, newCol)) {
                moves.push_back(Move(row, col, newRow, newCol, MoveType::Normal));
            } else if (board.isEnemyPiece(newRow, newCol, color)) {
                moves.push_back(Move(row, col, newRow, newCol, MoveType::Capture));
            }
        }
    }

    // Castling
    if (!hasMoved) {
        int homeRow = (color == Color::White) ? 7 : 0;

        // Kingside castling
        if (board.canCastleKingside(color)) {
            Piece* rook = board.getPiece(homeRow, 7);
            if (rook && rook->getType() == PieceType::Rook && !rook->getHasMoved()) {
                // Check if squares between are empty
                if (board.isEmpty(homeRow, 5) && board.isEmpty(homeRow, 6)) {
                    moves.push_back(Move(row, col, homeRow, 6, MoveType::CastleKingside));
                }
            }
        }

        // Queenside castling
        if (board.canCastleQueenside(color)) {
            Piece* rook = board.getPiece(homeRow, 0);
            if (rook && rook->getType() == PieceType::Rook && !rook->getHasMoved()) {
                // Check if squares between are empty
                if (board.isEmpty(homeRow, 1) && board.isEmpty(homeRow, 2) && board.isEmpty(homeRow, 3)) {
                    moves.push_back(Move(row, col, homeRow, 2, MoveType::CastleQueenside));
                }
            }
        }
    }

    return moves;
}

Piece* King::clone() const {
    King* copy = new King(color, row, col);
    copy->hasMoved = hasMoved;
    return copy;
}
