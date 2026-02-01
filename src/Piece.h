#pragma once

#include <vector>
#include <string>

enum class PieceType {
    King,
    Queen,
    Rook,
    Bishop,
    Knight,
    Pawn,
    None
};

enum class Color {
    White,
    Black,
    None
};

class Board;
struct Move;

class Piece {
protected:
    PieceType type;
    Color color;
    int row, col;
    bool hasMoved;

public:
    Piece(PieceType t, Color c, int r, int co);
    virtual ~Piece() = default;

    virtual std::vector<Move> getPseudoLegalMoves(const Board& board) const = 0;
    virtual Piece* clone() const = 0;

    PieceType getType() const { return type; }
    Color getColor() const { return color; }
    int getRow() const { return row; }
    int getCol() const { return col; }
    bool getHasMoved() const { return hasMoved; }

    void setPosition(int r, int c) { row = r; col = c; }
    void setHasMoved(bool moved) { hasMoved = moved; }

    wchar_t getSymbol() const;
    int getValue() const;

    static Color oppositeColor(Color c);
    static bool isValidSquare(int r, int c);
};
