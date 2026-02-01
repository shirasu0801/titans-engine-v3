#pragma once

#include <cstdint>

enum class MoveType {
    Normal,
    Capture,
    DoublePawnPush,
    EnPassant,
    CastleKingside,
    CastleQueenside,
    Promotion,
    PromotionCapture
};

enum class PieceType;

struct Move {
    int fromRow, fromCol;
    int toRow, toCol;
    MoveType type;
    PieceType promotionPiece;

    Move();
    Move(int fr, int fc, int tr, int tc, MoveType t = MoveType::Normal);

    bool operator==(const Move& other) const;
    bool operator!=(const Move& other) const;

    bool isValid() const;
    bool isCapture() const;
    bool isPromotion() const;
    bool isCastle() const;
};
