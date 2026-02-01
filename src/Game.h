#pragma once

#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Renderer.h"
#include "AI.h"
#include <memory>

enum class GameState {
    PlayerTurn,
    AIThinking,
    Promotion,
    Checkmate,
    Stalemate,
    Draw
};

class Game {
private:
    sf::RenderWindow window;
    Board board;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<AI> ai;

    GameState state;
    Color playerColor;
    Color aiColor;

    // Selection state
    int selectedRow;
    int selectedCol;
    std::vector<Move> currentLegalMoves;

    // Promotion state
    Move pendingPromotionMove;
    int promotionColumn;

    // Move history
    Move lastMove;

    void handleEvents();
    void handleMouseClick(int x, int y);
    void handlePromotion(int x, int y);

    void selectPiece(int row, int col);
    void deselectPiece();
    void tryMove(int row, int col);

    void makePlayerMove(const Move& move);
    void makeAIMove();

    void checkGameEnd();
    std::string getStatusText() const;

    void update();
    void render();

public:
    Game();

    void run();
};
