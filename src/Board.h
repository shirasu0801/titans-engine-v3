#pragma once

#include "Piece.h"
#include "Move.h"
#include <vector>
#include <memory>

class Board {
private:
    Piece* squares[8][8];
    Color currentTurn;
    Move lastMove;

    // Castling rights
    bool whiteCanCastleKingside;
    bool whiteCanCastleQueenside;
    bool blackCanCastleKingside;
    bool blackCanCastleQueenside;

    // King positions for quick access
    int whiteKingRow, whiteKingCol;
    int blackKingRow, blackKingCol;

    void clearBoard();
    void updateKingPosition(Color color, int row, int col);
    void updateCastlingRights(const Move& move, Piece* movedPiece);

public:
    Board();
    Board(const Board& other);
    Board& operator=(const Board& other);
    ~Board();

    void setupInitialPosition();

    // Piece access
    Piece* getPiece(int row, int col) const;
    void setPiece(int row, int col, Piece* piece);
    void removePiece(int row, int col);

    // Move handling
    bool makeMove(const Move& move);
    void undoMove(const Move& move, Piece* capturedPiece, bool wasFirstMove);

    // Move generation
    std::vector<Move> getPseudoLegalMoves(Color color) const;
    std::vector<Move> getLegalMoves(Color color);
    std::vector<Move> getLegalMovesForPiece(int row, int col);

    // Game state
    Color getCurrentTurn() const { return currentTurn; }
    void setCurrentTurn(Color turn) { currentTurn = turn; }
    void switchTurn();
    Move getLastMove() const { return lastMove; }

    // Check detection
    bool isSquareAttacked(int row, int col, Color byColor) const;
    bool isInCheck(Color color) const;
    bool isCheckmate(Color color);
    bool isStalemate(Color color);
    bool isDraw();

    // King position
    void getKingPosition(Color color, int& row, int& col) const;

    // Castling
    bool canCastleKingside(Color color) const;
    bool canCastleQueenside(Color color) const;

    // En passant
    bool canEnPassant(int pawnRow, int pawnCol, int targetCol) const;

    // Utility
    bool isEmpty(int row, int col) const;
    bool isEnemyPiece(int row, int col, Color myColor) const;
    bool isFriendlyPiece(int row, int col, Color myColor) const;

    // Evaluation helpers
    int countPieces(Color color) const;
    int getMaterialValue(Color color) const;
};
