#include "AI.h"
#include <algorithm>
#include <random>
#include <chrono>

// Piece-Square Tables (from white's perspective, will be mirrored for black)
// Values encourage good piece placement

const int AI::pawnTable[8][8] = {
    {  0,   0,   0,   0,   0,   0,   0,   0},
    { 50,  50,  50,  50,  50,  50,  50,  50},
    { 10,  10,  20,  30,  30,  20,  10,  10},
    {  5,   5,  10,  25,  25,  10,   5,   5},
    {  0,   0,   0,  20,  20,   0,   0,   0},
    {  5,  -5, -10,   0,   0, -10,  -5,   5},
    {  5,  10,  10, -20, -20,  10,  10,   5},
    {  0,   0,   0,   0,   0,   0,   0,   0}
};

const int AI::knightTable[8][8] = {
    {-50, -40, -30, -30, -30, -30, -40, -50},
    {-40, -20,   0,   0,   0,   0, -20, -40},
    {-30,   0,  10,  15,  15,  10,   0, -30},
    {-30,   5,  15,  20,  20,  15,   5, -30},
    {-30,   0,  15,  20,  20,  15,   0, -30},
    {-30,   5,  10,  15,  15,  10,   5, -30},
    {-40, -20,   0,   5,   5,   0, -20, -40},
    {-50, -40, -30, -30, -30, -30, -40, -50}
};

const int AI::bishopTable[8][8] = {
    {-20, -10, -10, -10, -10, -10, -10, -20},
    {-10,   0,   0,   0,   0,   0,   0, -10},
    {-10,   0,   5,  10,  10,   5,   0, -10},
    {-10,   5,   5,  10,  10,   5,   5, -10},
    {-10,   0,  10,  10,  10,  10,   0, -10},
    {-10,  10,  10,  10,  10,  10,  10, -10},
    {-10,   5,   0,   0,   0,   0,   5, -10},
    {-20, -10, -10, -10, -10, -10, -10, -20}
};

const int AI::rookTable[8][8] = {
    {  0,   0,   0,   0,   0,   0,   0,   0},
    {  5,  10,  10,  10,  10,  10,  10,   5},
    { -5,   0,   0,   0,   0,   0,   0,  -5},
    { -5,   0,   0,   0,   0,   0,   0,  -5},
    { -5,   0,   0,   0,   0,   0,   0,  -5},
    { -5,   0,   0,   0,   0,   0,   0,  -5},
    { -5,   0,   0,   0,   0,   0,   0,  -5},
    {  0,   0,   0,   5,   5,   0,   0,   0}
};

const int AI::queenTable[8][8] = {
    {-20, -10, -10,  -5,  -5, -10, -10, -20},
    {-10,   0,   0,   0,   0,   0,   0, -10},
    {-10,   0,   5,   5,   5,   5,   0, -10},
    { -5,   0,   5,   5,   5,   5,   0,  -5},
    {  0,   0,   5,   5,   5,   5,   0,  -5},
    {-10,   5,   5,   5,   5,   5,   0, -10},
    {-10,   0,   5,   0,   0,   0,   0, -10},
    {-20, -10, -10,  -5,  -5, -10, -10, -20}
};

const int AI::kingMiddleGameTable[8][8] = {
    {-30, -40, -40, -50, -50, -40, -40, -30},
    {-30, -40, -40, -50, -50, -40, -40, -30},
    {-30, -40, -40, -50, -50, -40, -40, -30},
    {-30, -40, -40, -50, -50, -40, -40, -30},
    {-20, -30, -30, -40, -40, -30, -30, -20},
    {-10, -20, -20, -20, -20, -20, -20, -10},
    { 20,  20,   0,   0,   0,   0,  20,  20},
    { 20,  30,  10,   0,   0,  10,  30,  20}
};

const int AI::kingEndGameTable[8][8] = {
    {-50, -40, -30, -20, -20, -30, -40, -50},
    {-30, -20, -10,   0,   0, -10, -20, -30},
    {-30, -10,  20,  30,  30,  20, -10, -30},
    {-30, -10,  30,  40,  40,  30, -10, -30},
    {-30, -10,  30,  40,  40,  30, -10, -30},
    {-30, -10,  20,  30,  30,  20, -10, -30},
    {-30, -30,   0,   0,   0,   0, -30, -30},
    {-50, -30, -30, -30, -30, -30, -30, -50}
};

AI::AI(Color color, int depth) : aiColor(color), maxDepth(depth) {}

bool AI::isEndGame(const Board& board) const {
    int totalMaterial = 0;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = board.getPiece(r, c);
            if (p && p->getType() != PieceType::King && p->getType() != PieceType::Pawn) {
                totalMaterial += p->getValue();
            }
        }
    }
    return totalMaterial < 2600; // Queens and a few pieces
}

int AI::getPieceSquareValue(const Piece* piece, bool endGame) const {
    if (!piece) return 0;

    int row = piece->getRow();
    int col = piece->getCol();

    // Mirror for black pieces
    if (piece->getColor() == Color::Black) {
        row = 7 - row;
    }

    switch (piece->getType()) {
        case PieceType::Pawn:   return pawnTable[row][col];
        case PieceType::Knight: return knightTable[row][col];
        case PieceType::Bishop: return bishopTable[row][col];
        case PieceType::Rook:   return rookTable[row][col];
        case PieceType::Queen:  return queenTable[row][col];
        case PieceType::King:
            return endGame ? kingEndGameTable[row][col] : kingMiddleGameTable[row][col];
        default: return 0;
    }
}

int AI::evaluate(const Board& board) const {
    int score = 0;
    bool endGame = isEndGame(board);

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* piece = board.getPiece(r, c);
            if (!piece) continue;

            int pieceValue = piece->getValue() + getPieceSquareValue(piece, endGame);

            if (piece->getColor() == aiColor) {
                score += pieceValue;
            } else {
                score -= pieceValue;
            }
        }
    }

    return score;
}

void AI::orderMoves(std::vector<Move>& moves, Board& board) {
    // Simple move ordering: captures first, then others
    std::sort(moves.begin(), moves.end(), [&board](const Move& a, const Move& b) {
        int scoreA = 0, scoreB = 0;

        // Prioritize captures
        if (a.isCapture()) {
            Piece* captured = board.getPiece(a.toRow, a.toCol);
            Piece* attacker = board.getPiece(a.fromRow, a.fromCol);
            if (captured && attacker) {
                // MVV-LVA (Most Valuable Victim - Least Valuable Attacker)
                scoreA = captured->getValue() * 10 - attacker->getValue();
            }
        }

        if (b.isCapture()) {
            Piece* captured = board.getPiece(b.toRow, b.toCol);
            Piece* attacker = board.getPiece(b.fromRow, b.fromCol);
            if (captured && attacker) {
                scoreB = captured->getValue() * 10 - attacker->getValue();
            }
        }

        // Prioritize promotions
        if (a.isPromotion()) scoreA += 800;
        if (b.isPromotion()) scoreB += 800;

        return scoreA > scoreB;
    });
}

int AI::minimax(Board& board, int depth, int alpha, int beta, bool maximizing) {
    Color currentColor = maximizing ? aiColor : Piece::oppositeColor(aiColor);

    // Terminal conditions
    if (board.isCheckmate(currentColor)) {
        return maximizing ? -100000 + (maxDepth - depth) : 100000 - (maxDepth - depth);
    }
    if (board.isStalemate(currentColor) || board.isDraw()) {
        return 0;
    }
    if (depth == 0) {
        return evaluate(board);
    }

    auto moves = board.getLegalMoves(currentColor);
    orderMoves(moves, board);

    if (maximizing) {
        int maxEval = std::numeric_limits<int>::min();
        for (const auto& move : moves) {
            Board newBoard(board);
            newBoard.makeMove(move);
            int eval = minimax(newBoard, depth - 1, alpha, beta, false);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) break;
        }
        return maxEval;
    } else {
        int minEval = std::numeric_limits<int>::max();
        for (const auto& move : moves) {
            Board newBoard(board);
            newBoard.makeMove(move);
            int eval = minimax(newBoard, depth - 1, alpha, beta, true);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) break;
        }
        return minEval;
    }
}

Move AI::getBestMove(Board& board) {
    auto moves = board.getLegalMoves(aiColor);

    if (moves.empty()) {
        return Move(); // No valid moves
    }

    orderMoves(moves, board);

    Move bestMove = moves[0];
    int bestScore = std::numeric_limits<int>::min();

    for (const auto& move : moves) {
        Board newBoard(board);
        newBoard.makeMove(move);
        int score = minimax(newBoard, maxDepth - 1,
                           std::numeric_limits<int>::min(),
                           std::numeric_limits<int>::max(),
                           false);

        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }

    // Add some randomness among equally good moves
    std::vector<Move> equalMoves;
    for (const auto& move : moves) {
        Board newBoard(board);
        newBoard.makeMove(move);
        int score = minimax(newBoard, maxDepth - 1,
                           std::numeric_limits<int>::min(),
                           std::numeric_limits<int>::max(),
                           false);
        if (score == bestScore) {
            equalMoves.push_back(move);
        }
    }

    if (equalMoves.size() > 1) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine rng(seed);
        std::uniform_int_distribution<size_t> dist(0, equalMoves.size() - 1);
        bestMove = equalMoves[dist(rng)];
    }

    return bestMove;
}
