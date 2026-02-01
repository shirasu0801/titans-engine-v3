#include "Piece.h"
#include "Move.h"

Piece::Piece(PieceType t, Color c, int r, int co)
    : type(t), color(c), row(r), col(co), hasMoved(false) {}

wchar_t Piece::getSymbol() const {
    if (color == Color::White) {
        switch (type) {
            case PieceType::King:   return L'♔';
            case PieceType::Queen:  return L'♕';
            case PieceType::Rook:   return L'♖';
            case PieceType::Bishop: return L'♗';
            case PieceType::Knight: return L'♘';
            case PieceType::Pawn:   return L'♙';
            default: return L' ';
        }
    } else {
        switch (type) {
            case PieceType::King:   return L'♚';
            case PieceType::Queen:  return L'♛';
            case PieceType::Rook:   return L'♜';
            case PieceType::Bishop: return L'♝';
            case PieceType::Knight: return L'♞';
            case PieceType::Pawn:   return L'♟';
            default: return L' ';
        }
    }
}

int Piece::getValue() const {
    switch (type) {
        case PieceType::Pawn:   return 100;
        case PieceType::Knight: return 320;
        case PieceType::Bishop: return 330;
        case PieceType::Rook:   return 500;
        case PieceType::Queen:  return 900;
        case PieceType::King:   return 20000;
        default: return 0;
    }
}

Color Piece::oppositeColor(Color c) {
    if (c == Color::White) return Color::Black;
    if (c == Color::Black) return Color::White;
    return Color::None;
}

bool Piece::isValidSquare(int r, int c) {
    return r >= 0 && r < 8 && c >= 0 && c < 8;
}
