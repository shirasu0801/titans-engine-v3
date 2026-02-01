#include "Board.h"
#include "Pieces/King.h"
#include "Pieces/Queen.h"
#include "Pieces/Rook.h"
#include "Pieces/Bishop.h"
#include "Pieces/Knight.h"
#include "Pieces/Pawn.h"

Board::Board()
    : currentTurn(Color::White),
      whiteCanCastleKingside(true), whiteCanCastleQueenside(true),
      blackCanCastleKingside(true), blackCanCastleQueenside(true),
      whiteKingRow(7), whiteKingCol(4),
      blackKingRow(0), blackKingCol(4) {
    // Initialize all squares to nullptr first
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            squares[r][c] = nullptr;
        }
    }
}

Board::Board(const Board& other)
    : currentTurn(other.currentTurn),
      lastMove(other.lastMove),
      whiteCanCastleKingside(other.whiteCanCastleKingside),
      whiteCanCastleQueenside(other.whiteCanCastleQueenside),
      blackCanCastleKingside(other.blackCanCastleKingside),
      blackCanCastleQueenside(other.blackCanCastleQueenside),
      whiteKingRow(other.whiteKingRow), whiteKingCol(other.whiteKingCol),
      blackKingRow(other.blackKingRow), blackKingCol(other.blackKingCol) {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (other.squares[r][c]) {
                squares[r][c] = other.squares[r][c]->clone();
            } else {
                squares[r][c] = nullptr;
            }
        }
    }
}

Board& Board::operator=(const Board& other) {
    if (this != &other) {
        clearBoard();
        currentTurn = other.currentTurn;
        lastMove = other.lastMove;
        whiteCanCastleKingside = other.whiteCanCastleKingside;
        whiteCanCastleQueenside = other.whiteCanCastleQueenside;
        blackCanCastleKingside = other.blackCanCastleKingside;
        blackCanCastleQueenside = other.blackCanCastleQueenside;
        whiteKingRow = other.whiteKingRow;
        whiteKingCol = other.whiteKingCol;
        blackKingRow = other.blackKingRow;
        blackKingCol = other.blackKingCol;

        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                if (other.squares[r][c]) {
                    squares[r][c] = other.squares[r][c]->clone();
                } else {
                    squares[r][c] = nullptr;
                }
            }
        }
    }
    return *this;
}

Board::~Board() {
    clearBoard();
}

void Board::clearBoard() {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            delete squares[r][c];
            squares[r][c] = nullptr;
        }
    }
}

void Board::setupInitialPosition() {
    clearBoard();

    // Black pieces (top)
    squares[0][0] = new Rook(Color::Black, 0, 0);
    squares[0][1] = new Knight(Color::Black, 0, 1);
    squares[0][2] = new Bishop(Color::Black, 0, 2);
    squares[0][3] = new Queen(Color::Black, 0, 3);
    squares[0][4] = new King(Color::Black, 0, 4);
    squares[0][5] = new Bishop(Color::Black, 0, 5);
    squares[0][6] = new Knight(Color::Black, 0, 6);
    squares[0][7] = new Rook(Color::Black, 0, 7);

    for (int c = 0; c < 8; ++c) {
        squares[1][c] = new Pawn(Color::Black, 1, c);
    }

    // White pieces (bottom)
    squares[7][0] = new Rook(Color::White, 7, 0);
    squares[7][1] = new Knight(Color::White, 7, 1);
    squares[7][2] = new Bishop(Color::White, 7, 2);
    squares[7][3] = new Queen(Color::White, 7, 3);
    squares[7][4] = new King(Color::White, 7, 4);
    squares[7][5] = new Bishop(Color::White, 7, 5);
    squares[7][6] = new Knight(Color::White, 7, 6);
    squares[7][7] = new Rook(Color::White, 7, 7);

    for (int c = 0; c < 8; ++c) {
        squares[6][c] = new Pawn(Color::White, 6, c);
    }

    currentTurn = Color::White;
    whiteCanCastleKingside = true;
    whiteCanCastleQueenside = true;
    blackCanCastleKingside = true;
    blackCanCastleQueenside = true;
    whiteKingRow = 7;
    whiteKingCol = 4;
    blackKingRow = 0;
    blackKingCol = 4;
}

Piece* Board::getPiece(int row, int col) const {
    if (!Piece::isValidSquare(row, col)) return nullptr;
    return squares[row][col];
}

void Board::setPiece(int row, int col, Piece* piece) {
    if (!Piece::isValidSquare(row, col)) return;
    squares[row][col] = piece;
    if (piece) {
        piece->setPosition(row, col);
    }
}

void Board::removePiece(int row, int col) {
    if (!Piece::isValidSquare(row, col)) return;
    delete squares[row][col];
    squares[row][col] = nullptr;
}

void Board::updateKingPosition(Color color, int row, int col) {
    if (color == Color::White) {
        whiteKingRow = row;
        whiteKingCol = col;
    } else {
        blackKingRow = row;
        blackKingCol = col;
    }
}

void Board::updateCastlingRights(const Move& move, Piece* movedPiece) {
    if (!movedPiece) return;

    Color color = movedPiece->getColor();
    PieceType type = movedPiece->getType();

    // King moved
    if (type == PieceType::King) {
        if (color == Color::White) {
            whiteCanCastleKingside = false;
            whiteCanCastleQueenside = false;
        } else {
            blackCanCastleKingside = false;
            blackCanCastleQueenside = false;
        }
    }

    // Rook moved
    if (type == PieceType::Rook) {
        if (color == Color::White) {
            if (move.fromRow == 7 && move.fromCol == 0) {
                whiteCanCastleQueenside = false;
            } else if (move.fromRow == 7 && move.fromCol == 7) {
                whiteCanCastleKingside = false;
            }
        } else {
            if (move.fromRow == 0 && move.fromCol == 0) {
                blackCanCastleQueenside = false;
            } else if (move.fromRow == 0 && move.fromCol == 7) {
                blackCanCastleKingside = false;
            }
        }
    }

    // Rook captured
    if (move.toRow == 0 && move.toCol == 0) blackCanCastleQueenside = false;
    if (move.toRow == 0 && move.toCol == 7) blackCanCastleKingside = false;
    if (move.toRow == 7 && move.toCol == 0) whiteCanCastleQueenside = false;
    if (move.toRow == 7 && move.toCol == 7) whiteCanCastleKingside = false;
}

bool Board::makeMove(const Move& move) {
    Piece* piece = getPiece(move.fromRow, move.fromCol);
    if (!piece) return false;

    Color color = piece->getColor();

    // Handle en passant capture
    if (move.type == MoveType::EnPassant) {
        int capturedRow = (color == Color::White) ? move.toRow + 1 : move.toRow - 1;
        removePiece(capturedRow, move.toCol);
    }

    // Handle castling
    if (move.type == MoveType::CastleKingside) {
        // Move rook
        Piece* rook = getPiece(move.fromRow, 7);
        squares[move.fromRow][5] = rook;
        squares[move.fromRow][7] = nullptr;
        if (rook) {
            rook->setPosition(move.fromRow, 5);
            rook->setHasMoved(true);
        }
    } else if (move.type == MoveType::CastleQueenside) {
        // Move rook
        Piece* rook = getPiece(move.fromRow, 0);
        squares[move.fromRow][3] = rook;
        squares[move.fromRow][0] = nullptr;
        if (rook) {
            rook->setPosition(move.fromRow, 3);
            rook->setHasMoved(true);
        }
    }

    // Capture at destination (not en passant)
    if (move.type != MoveType::EnPassant) {
        delete squares[move.toRow][move.toCol];
    }

    // Move piece
    squares[move.toRow][move.toCol] = piece;
    squares[move.fromRow][move.fromCol] = nullptr;
    piece->setPosition(move.toRow, move.toCol);
    piece->setHasMoved(true);

    // Handle promotion
    if (move.isPromotion()) {
        Piece* promoted = nullptr;
        switch (move.promotionPiece) {
            case PieceType::Queen:
                promoted = new Queen(color, move.toRow, move.toCol);
                break;
            case PieceType::Rook:
                promoted = new Rook(color, move.toRow, move.toCol);
                break;
            case PieceType::Bishop:
                promoted = new Bishop(color, move.toRow, move.toCol);
                break;
            case PieceType::Knight:
                promoted = new Knight(color, move.toRow, move.toCol);
                break;
            default:
                promoted = new Queen(color, move.toRow, move.toCol);
                break;
        }
        promoted->setHasMoved(true);
        delete squares[move.toRow][move.toCol];
        squares[move.toRow][move.toCol] = promoted;
    }

    // Update king position
    if (piece->getType() == PieceType::King) {
        updateKingPosition(color, move.toRow, move.toCol);
    }

    updateCastlingRights(move, piece);
    lastMove = move;
    switchTurn();

    return true;
}

void Board::switchTurn() {
    currentTurn = Piece::oppositeColor(currentTurn);
}

std::vector<Move> Board::getPseudoLegalMoves(Color color) const {
    std::vector<Move> moves;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* piece = squares[r][c];
            if (piece && piece->getColor() == color) {
                auto pieceMoves = piece->getPseudoLegalMoves(*this);
                moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
            }
        }
    }
    return moves;
}

std::vector<Move> Board::getLegalMoves(Color color) {
    std::vector<Move> legalMoves;
    auto pseudoLegalMoves = getPseudoLegalMoves(color);

    for (const auto& move : pseudoLegalMoves) {
        // Make a copy of the board to test the move
        Board testBoard(*this);
        Piece* piece = testBoard.getPiece(move.fromRow, move.fromCol);
        if (!piece) continue;

        // Handle en passant capture
        if (move.type == MoveType::EnPassant) {
            int capturedRow = (color == Color::White) ? move.toRow + 1 : move.toRow - 1;
            testBoard.removePiece(capturedRow, move.toCol);
        }

        // Handle castling - check if squares are attacked
        if (move.type == MoveType::CastleKingside) {
            int row = (color == Color::White) ? 7 : 0;
            if (testBoard.isSquareAttacked(row, 4, Piece::oppositeColor(color)) ||
                testBoard.isSquareAttacked(row, 5, Piece::oppositeColor(color)) ||
                testBoard.isSquareAttacked(row, 6, Piece::oppositeColor(color))) {
                continue;
            }
        } else if (move.type == MoveType::CastleQueenside) {
            int row = (color == Color::White) ? 7 : 0;
            if (testBoard.isSquareAttacked(row, 4, Piece::oppositeColor(color)) ||
                testBoard.isSquareAttacked(row, 3, Piece::oppositeColor(color)) ||
                testBoard.isSquareAttacked(row, 2, Piece::oppositeColor(color))) {
                continue;
            }
        }

        // Make the move
        delete testBoard.squares[move.toRow][move.toCol];
        testBoard.squares[move.toRow][move.toCol] = testBoard.squares[move.fromRow][move.fromCol];
        testBoard.squares[move.fromRow][move.fromCol] = nullptr;
        testBoard.squares[move.toRow][move.toCol]->setPosition(move.toRow, move.toCol);

        // Update king position if king moved
        if (piece->getType() == PieceType::King) {
            testBoard.updateKingPosition(color, move.toRow, move.toCol);
        }

        // Check if own king is in check after the move
        if (!testBoard.isInCheck(color)) {
            legalMoves.push_back(move);
        }
    }

    return legalMoves;
}

std::vector<Move> Board::getLegalMovesForPiece(int row, int col) {
    Piece* piece = getPiece(row, col);
    if (!piece) return {};

    Color color = piece->getColor();
    auto allLegalMoves = getLegalMoves(color);

    std::vector<Move> pieceMoves;
    for (const auto& move : allLegalMoves) {
        if (move.fromRow == row && move.fromCol == col) {
            pieceMoves.push_back(move);
        }
    }

    return pieceMoves;
}

bool Board::isSquareAttacked(int row, int col, Color byColor) const {
    // Check pawn attacks
    int pawnDir = (byColor == Color::White) ? 1 : -1;
    int pawnRow = row + pawnDir;
    if (Piece::isValidSquare(pawnRow, col - 1)) {
        Piece* p = squares[pawnRow][col - 1];
        if (p && p->getType() == PieceType::Pawn && p->getColor() == byColor) {
            return true;
        }
    }
    if (Piece::isValidSquare(pawnRow, col + 1)) {
        Piece* p = squares[pawnRow][col + 1];
        if (p && p->getType() == PieceType::Pawn && p->getColor() == byColor) {
            return true;
        }
    }

    // Check knight attacks
    int knightMoves[8][2] = {
        {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
        {1, -2}, {1, 2}, {2, -1}, {2, 1}
    };
    for (const auto& km : knightMoves) {
        int r = row + km[0];
        int c = col + km[1];
        if (Piece::isValidSquare(r, c)) {
            Piece* p = squares[r][c];
            if (p && p->getType() == PieceType::Knight && p->getColor() == byColor) {
                return true;
            }
        }
    }

    // Check king attacks
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) continue;
            int r = row + dr;
            int c = col + dc;
            if (Piece::isValidSquare(r, c)) {
                Piece* p = squares[r][c];
                if (p && p->getType() == PieceType::King && p->getColor() == byColor) {
                    return true;
                }
            }
        }
    }

    // Check rook/queen attacks (straight lines)
    int straightDirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (const auto& dir : straightDirs) {
        for (int i = 1; i < 8; ++i) {
            int r = row + dir[0] * i;
            int c = col + dir[1] * i;
            if (!Piece::isValidSquare(r, c)) break;
            Piece* p = squares[r][c];
            if (p) {
                if (p->getColor() == byColor &&
                    (p->getType() == PieceType::Rook || p->getType() == PieceType::Queen)) {
                    return true;
                }
                break;
            }
        }
    }

    // Check bishop/queen attacks (diagonals)
    int diagDirs[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    for (const auto& dir : diagDirs) {
        for (int i = 1; i < 8; ++i) {
            int r = row + dir[0] * i;
            int c = col + dir[1] * i;
            if (!Piece::isValidSquare(r, c)) break;
            Piece* p = squares[r][c];
            if (p) {
                if (p->getColor() == byColor &&
                    (p->getType() == PieceType::Bishop || p->getType() == PieceType::Queen)) {
                    return true;
                }
                break;
            }
        }
    }

    return false;
}

bool Board::isInCheck(Color color) const {
    int kingRow, kingCol;
    getKingPosition(color, kingRow, kingCol);
    return isSquareAttacked(kingRow, kingCol, Piece::oppositeColor(color));
}

bool Board::isCheckmate(Color color) {
    if (!isInCheck(color)) return false;
    return getLegalMoves(color).empty();
}

bool Board::isStalemate(Color color) {
    if (isInCheck(color)) return false;
    return getLegalMoves(color).empty();
}

bool Board::isDraw() {
    // Insufficient material check (simplified)
    int whitePieces = 0, blackPieces = 0;
    int whiteMinor = 0, blackMinor = 0;

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = squares[r][c];
            if (!p) continue;
            if (p->getColor() == Color::White) {
                if (p->getType() != PieceType::King) whitePieces++;
                if (p->getType() == PieceType::Knight || p->getType() == PieceType::Bishop) {
                    whiteMinor++;
                }
            } else {
                if (p->getType() != PieceType::King) blackPieces++;
                if (p->getType() == PieceType::Knight || p->getType() == PieceType::Bishop) {
                    blackMinor++;
                }
            }
        }
    }

    // King vs King
    if (whitePieces == 0 && blackPieces == 0) return true;
    // King + minor piece vs King
    if ((whitePieces == 1 && whiteMinor == 1 && blackPieces == 0) ||
        (blackPieces == 1 && blackMinor == 1 && whitePieces == 0)) {
        return true;
    }

    return false;
}

void Board::getKingPosition(Color color, int& row, int& col) const {
    if (color == Color::White) {
        row = whiteKingRow;
        col = whiteKingCol;
    } else {
        row = blackKingRow;
        col = blackKingCol;
    }
}

bool Board::canCastleKingside(Color color) const {
    if (color == Color::White) {
        return whiteCanCastleKingside;
    } else {
        return blackCanCastleKingside;
    }
}

bool Board::canCastleQueenside(Color color) const {
    if (color == Color::White) {
        return whiteCanCastleQueenside;
    } else {
        return blackCanCastleQueenside;
    }
}

bool Board::canEnPassant(int pawnRow, int pawnCol, int targetCol) const {
    if (!lastMove.isValid()) return false;
    if (lastMove.type != MoveType::DoublePawnPush) return false;

    Piece* lastMovedPiece = getPiece(lastMove.toRow, lastMove.toCol);
    if (!lastMovedPiece || lastMovedPiece->getType() != PieceType::Pawn) return false;

    // Check if the target column matches the last pawn's column
    if (lastMove.toCol != targetCol) return false;

    // Check if pawns are adjacent
    if (lastMove.toRow != pawnRow) return false;
    if (std::abs(lastMove.toCol - pawnCol) != 1) return false;

    return true;
}

bool Board::isEmpty(int row, int col) const {
    return getPiece(row, col) == nullptr;
}

bool Board::isEnemyPiece(int row, int col, Color myColor) const {
    Piece* p = getPiece(row, col);
    return p && p->getColor() != myColor;
}

bool Board::isFriendlyPiece(int row, int col, Color myColor) const {
    Piece* p = getPiece(row, col);
    return p && p->getColor() == myColor;
}

int Board::countPieces(Color color) const {
    int count = 0;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = squares[r][c];
            if (p && p->getColor() == color) {
                count++;
            }
        }
    }
    return count;
}

int Board::getMaterialValue(Color color) const {
    int value = 0;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = squares[r][c];
            if (p && p->getColor() == color) {
                value += p->getValue();
            }
        }
    }
    return value;
}
