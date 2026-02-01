#include "Renderer.h"
#include <iostream>

Renderer::Renderer(sf::RenderWindow& win, int sqSize)
    : window(win), squareSize(sqSize), fontLoaded(false) {

    boardOffsetX = 40;
    boardOffsetY = 40;

    lightSquare = sf::Color(240, 217, 181);
    darkSquare = sf::Color(181, 136, 99);
    selectedColor = sf::Color(130, 151, 105, 200);
    legalMoveColor = sf::Color(130, 151, 105, 150);
    lastMoveColor = sf::Color(205, 210, 106, 150);
    checkColor = sf::Color(255, 0, 0, 100);
}

void Renderer::loadFont(const std::string& fontPath) {
    // Try to load the specified font
    try {
        if (font.openFromFile(fontPath)) {
            fontLoaded = true;
            return;
        }
    } catch (...) {}

    // Try common Windows fonts that support chess symbols
    std::vector<std::string> fallbackFonts = {
        "C:/Windows/Fonts/seguisym.ttf",  // Segoe UI Symbol
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/arialuni.ttf",  // Arial Unicode MS
        "C:/Windows/Fonts/times.ttf",
        "C:/Windows/Fonts/DejaVuSans.ttf"
    };

    for (const auto& fallback : fallbackFonts) {
        try {
            if (font.openFromFile(fallback)) {
                fontLoaded = true;
                return;
            }
        } catch (...) {}
    }

    std::cerr << "Warning: Could not load any font. Pieces will be shown as colored circles." << std::endl;
}

void Renderer::drawBoard() {
    // Draw squares
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            sf::RectangleShape square(sf::Vector2f(static_cast<float>(squareSize), static_cast<float>(squareSize)));
            square.setPosition(sf::Vector2f(static_cast<float>(boardOffsetX + col * squareSize),
                                            static_cast<float>(boardOffsetY + row * squareSize)));

            if ((row + col) % 2 == 0) {
                square.setFillColor(lightSquare);
            } else {
                square.setFillColor(darkSquare);
            }

            window.draw(square);
        }
    }

    // Draw coordinates
    if (fontLoaded) {
        for (int i = 0; i < 8; ++i) {
            // File letters (a-h)
            sf::Text fileLetter(font, std::string(1, 'a' + i), 16);
            fileLetter.setFillColor(sf::Color::Black);
            fileLetter.setPosition(sf::Vector2f(static_cast<float>(boardOffsetX + i * squareSize + squareSize / 2 - 5),
                                                static_cast<float>(boardOffsetY + 8 * squareSize + 5)));
            window.draw(fileLetter);

            // Rank numbers (8-1)
            sf::Text rankNumber(font, std::to_string(8 - i), 16);
            rankNumber.setFillColor(sf::Color::Black);
            rankNumber.setPosition(sf::Vector2f(static_cast<float>(boardOffsetX - 20),
                                                static_cast<float>(boardOffsetY + i * squareSize + squareSize / 2 - 10)));
            window.draw(rankNumber);
        }
    }
}

void Renderer::drawPieces(const Board& board) {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Piece* piece = board.getPiece(row, col);
            if (!piece) continue;

            float x = static_cast<float>(boardOffsetX + col * squareSize + squareSize / 2);
            float y = static_cast<float>(boardOffsetY + row * squareSize + squareSize / 2);

            if (fontLoaded) {
                // Use Unicode chess symbols
                wchar_t symbol = piece->getSymbol();
                sf::Text pieceText(font, sf::String(symbol), static_cast<unsigned int>(squareSize - 15));

                // Set color with outline for visibility
                if (piece->getColor() == Color::White) {
                    pieceText.setFillColor(sf::Color::White);
                    pieceText.setOutlineColor(sf::Color::Black);
                    pieceText.setOutlineThickness(1.5f);
                } else {
                    pieceText.setFillColor(sf::Color::Black);
                    pieceText.setOutlineColor(sf::Color::White);
                    pieceText.setOutlineThickness(0.5f);
                }

                // Center the text
                sf::FloatRect bounds = pieceText.getLocalBounds();
                pieceText.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.0f,
                                                  bounds.position.y + bounds.size.y / 2.0f));
                pieceText.setPosition(sf::Vector2f(x, y));

                window.draw(pieceText);
            } else {
                // Fallback: draw colored circles
                sf::CircleShape circle(static_cast<float>(squareSize) / 2.5f);
                circle.setOrigin(sf::Vector2f(static_cast<float>(squareSize) / 2.5f,
                                              static_cast<float>(squareSize) / 2.5f));
                circle.setPosition(sf::Vector2f(x, y));

                if (piece->getColor() == Color::White) {
                    circle.setFillColor(sf::Color::White);
                    circle.setOutlineColor(sf::Color::Black);
                } else {
                    circle.setFillColor(sf::Color::Black);
                    circle.setOutlineColor(sf::Color::White);
                }
                circle.setOutlineThickness(2);

                window.draw(circle);
            }
        }
    }
}

void Renderer::drawSelection(int row, int col) {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) return;

    sf::RectangleShape highlight(sf::Vector2f(static_cast<float>(squareSize), static_cast<float>(squareSize)));
    highlight.setPosition(sf::Vector2f(static_cast<float>(boardOffsetX + col * squareSize),
                                       static_cast<float>(boardOffsetY + row * squareSize)));
    highlight.setFillColor(selectedColor);
    window.draw(highlight);
}

void Renderer::drawLegalMoves(const std::vector<Move>& moves) {
    for (const auto& move : moves) {
        float centerX = static_cast<float>(boardOffsetX + move.toCol * squareSize + squareSize / 2);
        float centerY = static_cast<float>(boardOffsetY + move.toRow * squareSize + squareSize / 2);

        if (move.isCapture()) {
            // Draw a ring for captures
            float radius = static_cast<float>(squareSize) / 2.0f - 5.0f;
            sf::CircleShape ring(radius);
            ring.setOrigin(sf::Vector2f(radius, radius));
            ring.setPosition(sf::Vector2f(centerX, centerY));
            ring.setFillColor(sf::Color::Transparent);
            ring.setOutlineColor(legalMoveColor);
            ring.setOutlineThickness(5);
            window.draw(ring);
        } else {
            // Draw a dot for normal moves
            float radius = static_cast<float>(squareSize) / 6.0f;
            sf::CircleShape dot(radius);
            dot.setOrigin(sf::Vector2f(radius, radius));
            dot.setPosition(sf::Vector2f(centerX, centerY));
            dot.setFillColor(legalMoveColor);
            window.draw(dot);
        }
    }
}

void Renderer::drawLastMove(const Move& move) {
    if (!move.isValid()) return;

    // Highlight from square
    sf::RectangleShape fromHighlight(sf::Vector2f(static_cast<float>(squareSize), static_cast<float>(squareSize)));
    fromHighlight.setPosition(sf::Vector2f(static_cast<float>(boardOffsetX + move.fromCol * squareSize),
                                           static_cast<float>(boardOffsetY + move.fromRow * squareSize)));
    fromHighlight.setFillColor(lastMoveColor);
    window.draw(fromHighlight);

    // Highlight to square
    sf::RectangleShape toHighlight(sf::Vector2f(static_cast<float>(squareSize), static_cast<float>(squareSize)));
    toHighlight.setPosition(sf::Vector2f(static_cast<float>(boardOffsetX + move.toCol * squareSize),
                                         static_cast<float>(boardOffsetY + move.toRow * squareSize)));
    toHighlight.setFillColor(lastMoveColor);
    window.draw(toHighlight);
}

void Renderer::drawCheck(const Board& board) {
    Color currentTurn = board.getCurrentTurn();
    if (!board.isInCheck(currentTurn)) return;

    int kingRow, kingCol;
    board.getKingPosition(currentTurn, kingRow, kingCol);

    sf::RectangleShape checkHighlight(sf::Vector2f(static_cast<float>(squareSize), static_cast<float>(squareSize)));
    checkHighlight.setPosition(sf::Vector2f(static_cast<float>(boardOffsetX + kingCol * squareSize),
                                            static_cast<float>(boardOffsetY + kingRow * squareSize)));
    checkHighlight.setFillColor(checkColor);
    window.draw(checkHighlight);
}

void Renderer::drawStatus(const std::string& text) {
    if (!fontLoaded) return;

    sf::Text statusText(font, text, 24);
    statusText.setFillColor(sf::Color::Black);
    statusText.setPosition(sf::Vector2f(static_cast<float>(boardOffsetX),
                                        static_cast<float>(boardOffsetY + 8 * squareSize + 30)));
    window.draw(statusText);
}

void Renderer::drawPromotionDialog(Color color, int col) {
    // Draw a vertical dialog showing promotion options
    float x = static_cast<float>(boardOffsetX + col * squareSize);
    float startY = (color == Color::White) ? static_cast<float>(boardOffsetY)
                                           : static_cast<float>(boardOffsetY + 4 * squareSize);

    // Background
    sf::RectangleShape background(sf::Vector2f(static_cast<float>(squareSize), static_cast<float>(squareSize * 4)));
    background.setPosition(sf::Vector2f(x, startY));
    background.setFillColor(sf::Color(200, 200, 200, 240));
    background.setOutlineColor(sf::Color::Black);
    background.setOutlineThickness(2);
    window.draw(background);

    // Piece options: Queen, Rook, Bishop, Knight
    wchar_t whiteSymbols[] = {L'♕', L'♖', L'♗', L'♘'};
    wchar_t blackSymbols[] = {L'♛', L'♜', L'♝', L'♞'};

    for (int i = 0; i < 4; ++i) {
        float pieceY = startY + static_cast<float>(i * squareSize + squareSize / 2);
        float pieceX = x + static_cast<float>(squareSize / 2);

        if (fontLoaded) {
            wchar_t symbol = (color == Color::White) ? whiteSymbols[i] : blackSymbols[i];
            sf::Text pieceText(font, sf::String(symbol), static_cast<unsigned int>(squareSize - 15));

            if (color == Color::White) {
                pieceText.setFillColor(sf::Color::White);
                pieceText.setOutlineColor(sf::Color::Black);
                pieceText.setOutlineThickness(1.5f);
            } else {
                pieceText.setFillColor(sf::Color::Black);
                pieceText.setOutlineColor(sf::Color::White);
                pieceText.setOutlineThickness(0.5f);
            }

            sf::FloatRect bounds = pieceText.getLocalBounds();
            pieceText.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.0f,
                                              bounds.position.y + bounds.size.y / 2.0f));
            pieceText.setPosition(sf::Vector2f(pieceX, pieceY));

            window.draw(pieceText);
        }
    }
}

void Renderer::render(const Board& board, int selectedRow, int selectedCol,
                      const std::vector<Move>& legalMoves, const Move& lastMove) {
    drawBoard();
    drawLastMove(lastMove);
    drawCheck(board);
    drawSelection(selectedRow, selectedCol);
    drawLegalMoves(legalMoves);
    drawPieces(board);
}

bool Renderer::getSquareFromMouse(int mouseX, int mouseY, int& row, int& col) const {
    int relX = mouseX - boardOffsetX;
    int relY = mouseY - boardOffsetY;

    if (relX < 0 || relX >= 8 * squareSize || relY < 0 || relY >= 8 * squareSize) {
        return false;
    }

    col = relX / squareSize;
    row = relY / squareSize;

    return true;
}

int Renderer::getPromotionChoice(int mouseY) const {
    // Returns 0-3 for Queen, Rook, Bishop, Knight
    int relY = mouseY - boardOffsetY;
    int choice = relY / squareSize;
    if (choice >= 0 && choice < 4) {
        return choice;
    }
    return 0; // Default to Queen
}
