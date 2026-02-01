#include "Pawn.h"
#include "../Board.h"
#include "../Move.h"

Pawn::Pawn(Color c, int r, int col) : Piece(PieceType::Pawn, c, r, col) {}

std::vector<Move> Pawn::getPseudoLegalMoves(const Board& board) const {
    std::vector<Move> moves;

    int direction = (color == Color::White) ? -1 : 1;  // White moves up, black moves down
    int startRow = (color == Color::White) ? 6 : 1;
    int promotionRow = (color == Color::White) ? 0 : 7;

    // Single step forward
    int newRow = row + direction;
    if (isValidSquare(newRow, col) && board.isEmpty(newRow, col)) {
        if (newRow == promotionRow) {
            addPromotionMoves(moves, row, col, newRow, col, false);
        } else {
            moves.push_back(Move(row, col, newRow, col, MoveType::Normal));

            // Double step from starting position
            if (row == startRow) {
                int doubleRow = row + 2 * direction;
                if (board.isEmpty(doubleRow, col)) {
                    moves.push_back(Move(row, col, doubleRow, col, MoveType::DoublePawnPush));
                }
            }
        }
    }

    // Diagonal captures
    for (int dc = -1; dc <= 1; dc += 2) {
        int newCol = col + dc;
        if (!isValidSquare(newRow, newCol)) continue;

        if (board.isEnemyPiece(newRow, newCol, color)) {
            if (newRow == promotionRow) {
                addPromotionMoves(moves, row, col, newRow, newCol, true);
            } else {
                moves.push_back(Move(row, col, newRow, newCol, MoveType::Capture));
            }
        }

        // En passant
        if (board.canEnPassant(row, col, newCol)) {
            moves.push_back(Move(row, col, newRow, newCol, MoveType::EnPassant));
        }
    }

    return moves;
}

void Pawn::addPromotionMoves(std::vector<Move>& moves, int fromRow, int fromCol,
                              int toRow, int toCol, bool isCapture) const {
    MoveType type = isCapture ? MoveType::PromotionCapture : MoveType::Promotion;

    Move queenPromo(fromRow, fromCol, toRow, toCol, type);
    queenPromo.promotionPiece = PieceType::Queen;
    moves.push_back(queenPromo);

    Move rookPromo(fromRow, fromCol, toRow, toCol, type);
    rookPromo.promotionPiece = PieceType::Rook;
    moves.push_back(rookPromo);

    Move bishopPromo(fromRow, fromCol, toRow, toCol, type);
    bishopPromo.promotionPiece = PieceType::Bishop;
    moves.push_back(bishopPromo);

    Move knightPromo(fromRow, fromCol, toRow, toCol, type);
    knightPromo.promotionPiece = PieceType::Knight;
    moves.push_back(knightPromo);
}

Piece* Pawn::clone() const {
    Pawn* copy = new Pawn(color, row, col);
    copy->hasMoved = hasMoved;
    return copy;
}
