// WASM Bridge for Chess Engine
// This file provides the JavaScript-C++ interface using Emscripten

#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#include <emscripten/val.h>
#endif

#include "../Board.h"
#include "../AI.h"
#include "../Move.h"
#include "../Piece.h"
#include <string>
#include <sstream>
#include <vector>

// Global game state
static Board* g_board = nullptr;
static AI* g_ai = nullptr;
static bool g_gameOver = false;
static std::string g_gameStatus = "playing";

// Initialize or reset the game
void initGame() {
    delete g_board;
    delete g_ai;

    g_board = new Board();
    g_board->setupInitialPosition();
    g_ai = new AI(Color::Black, 4);
    g_gameOver = false;
    g_gameStatus = "playing";
}

// Get board state as JSON string
std::string getBoardState() {
    if (!g_board) return "{}";

    std::ostringstream json;
    json << "{\"squares\":[";

    for (int row = 0; row < 8; row++) {
        json << "[";
        for (int col = 0; col < 8; col++) {
            Piece* piece = g_board->getPiece(row, col);
            if (piece) {
                json << "{\"type\":" << static_cast<int>(piece->getType())
                     << ",\"color\":" << static_cast<int>(piece->getColor()) << "}";
            } else {
                json << "null";
            }
            if (col < 7) json << ",";
        }
        json << "]";
        if (row < 7) json << ",";
    }

    json << "],\"currentTurn\":" << static_cast<int>(g_board->getCurrentTurn())
         << ",\"isCheck\":" << (g_board->isInCheck(g_board->getCurrentTurn()) ? "true" : "false")
         << ",\"gameOver\":" << (g_gameOver ? "true" : "false")
         << ",\"status\":\"" << g_gameStatus << "\""
         << "}";

    return json.str();
}

// Get legal moves for a piece at (row, col) as JSON
std::string getLegalMoves(int row, int col) {
    if (!g_board) return "[]";

    Piece* piece = g_board->getPiece(row, col);
    if (!piece || piece->getColor() != g_board->getCurrentTurn()) {
        return "[]";
    }

    std::vector<Move> moves = g_board->getLegalMovesForPiece(row, col);

    std::ostringstream json;
    json << "[";

    for (size_t i = 0; i < moves.size(); i++) {
        const Move& m = moves[i];
        json << "{\"toRow\":" << m.toRow
             << ",\"toCol\":" << m.toCol
             << ",\"type\":" << static_cast<int>(m.type)
             << ",\"isCapture\":" << (m.isCapture() ? "true" : "false")
             << ",\"isPromotion\":" << (m.isPromotion() ? "true" : "false")
             << ",\"isCastle\":" << (m.isCastle() ? "true" : "false")
             << "}";
        if (i < moves.size() - 1) json << ",";
    }

    json << "]";
    return json.str();
}

// Make a move, returns true if successful
bool makeMove(int fromRow, int fromCol, int toRow, int toCol, int promotionPiece) {
    if (!g_board || g_gameOver) return false;

    Piece* piece = g_board->getPiece(fromRow, fromCol);
    if (!piece || piece->getColor() != g_board->getCurrentTurn()) {
        return false;
    }

    // Find matching legal move
    std::vector<Move> legalMoves = g_board->getLegalMovesForPiece(fromRow, fromCol);

    for (const Move& m : legalMoves) {
        if (m.toRow == toRow && m.toCol == toCol) {
            // Found matching move
            Move moveToMake = m;

            // Handle promotion
            if (m.isPromotion() && promotionPiece >= 0) {
                moveToMake.promotionPiece = static_cast<PieceType>(promotionPiece);
            }

            g_board->makeMove(moveToMake);
            // Note: Board::makeMove already calls switchTurn() internally

            // Check game end conditions
            Color currentPlayer = g_board->getCurrentTurn();
            if (g_board->isCheckmate(currentPlayer)) {
                g_gameOver = true;
                g_gameStatus = (currentPlayer == Color::White) ? "black_wins" : "white_wins";
            } else if (g_board->isStalemate(currentPlayer)) {
                g_gameOver = true;
                g_gameStatus = "stalemate";
            } else if (g_board->isDraw()) {
                g_gameOver = true;
                g_gameStatus = "draw";
            }

            return true;
        }
    }

    return false;
}

// Get AI's best move as JSON
std::string getAIMove() {
    if (!g_board || !g_ai || g_gameOver) return "{}";

    Move bestMove = g_ai->getBestMove(*g_board);

    if (!bestMove.isValid()) return "{}";

    std::ostringstream json;
    json << "{\"fromRow\":" << bestMove.fromRow
         << ",\"fromCol\":" << bestMove.fromCol
         << ",\"toRow\":" << bestMove.toRow
         << ",\"toCol\":" << bestMove.toCol
         << ",\"type\":" << static_cast<int>(bestMove.type)
         << ",\"promotionPiece\":" << static_cast<int>(bestMove.promotionPiece)
         << "}";

    return json.str();
}

// Check if game is over
bool isGameOver() {
    return g_gameOver;
}

// Get current game status
std::string getGameStatus() {
    return g_gameStatus;
}

// Set AI depth
void setAIDepth(int depth) {
    if (g_ai) {
        g_ai->setDepth(depth);
    }
}

// Get current turn (0 = White, 1 = Black)
int getCurrentTurn() {
    if (!g_board) return 0;
    return static_cast<int>(g_board->getCurrentTurn());
}

// Check if a square is under attack
bool isSquareAttacked(int row, int col, int byColor) {
    if (!g_board) return false;
    return g_board->isSquareAttacked(row, col, static_cast<Color>(byColor));
}

// Get last move as JSON
std::string getLastMove() {
    if (!g_board) return "{}";

    Move lastMove = g_board->getLastMove();
    if (!lastMove.isValid()) return "{}";

    std::ostringstream json;
    json << "{\"fromRow\":" << lastMove.fromRow
         << ",\"fromCol\":" << lastMove.fromCol
         << ",\"toRow\":" << lastMove.toRow
         << ",\"toCol\":" << lastMove.toCol
         << "}";

    return json.str();
}

// Cleanup
void cleanup() {
    delete g_board;
    delete g_ai;
    g_board = nullptr;
    g_ai = nullptr;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(chess_module) {
    emscripten::function("initGame", &initGame);
    emscripten::function("getBoardState", &getBoardState);
    emscripten::function("getLegalMoves", &getLegalMoves);
    emscripten::function("makeMove", &makeMove);
    emscripten::function("getAIMove", &getAIMove);
    emscripten::function("isGameOver", &isGameOver);
    emscripten::function("getGameStatus", &getGameStatus);
    emscripten::function("setAIDepth", &setAIDepth);
    emscripten::function("getCurrentTurn", &getCurrentTurn);
    emscripten::function("isSquareAttacked", &isSquareAttacked);
    emscripten::function("getLastMove", &getLastMove);
    emscripten::function("cleanup", &cleanup);
}
#endif
