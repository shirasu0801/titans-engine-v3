#pragma once

#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Move.h"
#include <vector>
#include <string>

class Renderer {
private:
    sf::RenderWindow& window;
    sf::Font font;
    bool fontLoaded;

    int squareSize;
    int boardOffsetX;
    int boardOffsetY;

    // Colors
    sf::Color lightSquare;
    sf::Color darkSquare;
    sf::Color selectedColor;
    sf::Color legalMoveColor;
    sf::Color lastMoveColor;
    sf::Color checkColor;

    // Status bar
    std::string statusText;

public:
    Renderer(sf::RenderWindow& win, int sqSize = 80);

    void loadFont(const std::string& fontPath);

    void drawBoard();
    void drawPieces(const Board& board);
    void drawSelection(int row, int col);
    void drawLegalMoves(const std::vector<Move>& moves);
    void drawLastMove(const Move& move);
    void drawCheck(const Board& board);
    void drawStatus(const std::string& text);
    void drawPromotionDialog(Color color, int col);

    void render(const Board& board, int selectedRow, int selectedCol,
                const std::vector<Move>& legalMoves, const Move& lastMove);

    // Coordinate conversion
    bool getSquareFromMouse(int mouseX, int mouseY, int& row, int& col) const;
    int getPromotionChoice(int mouseY) const;

    int getSquareSize() const { return squareSize; }
    int getBoardOffsetX() const { return boardOffsetX; }
    int getBoardOffsetY() const { return boardOffsetY; }
};
