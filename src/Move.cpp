#include "Move.h"
#include "Piece.h"

Move::Move()
    : fromRow(-1), fromCol(-1), toRow(-1), toCol(-1),
      type(MoveType::Normal), promotionPiece(PieceType::Queen) {}

Move::Move(int fr, int fc, int tr, int tc, MoveType t)
    : fromRow(fr), fromCol(fc), toRow(tr), toCol(tc),
      type(t), promotionPiece(PieceType::Queen) {}

bool Move::operator==(const Move& other) const {
    return fromRow == other.fromRow && fromCol == other.fromCol &&
           toRow == other.toRow && toCol == other.toCol &&
           type == other.type;
}

bool Move::operator!=(const Move& other) const {
    return !(*this == other);
}

bool Move::isValid() const {
    return fromRow >= 0 && fromRow < 8 && fromCol >= 0 && fromCol < 8 &&
           toRow >= 0 && toRow < 8 && toCol >= 0 && toCol < 8;
}

bool Move::isCapture() const {
    return type == MoveType::Capture ||
           type == MoveType::EnPassant ||
           type == MoveType::PromotionCapture;
}

bool Move::isPromotion() const {
    return type == MoveType::Promotion || type == MoveType::PromotionCapture;
}

bool Move::isCastle() const {
    return type == MoveType::CastleKingside || type == MoveType::CastleQueenside;
}
