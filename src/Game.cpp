#include "Game.h"
#include <iostream>
#include <thread>

Game::Game()
    : state(GameState::PlayerTurn),
      playerColor(Color::White),
      aiColor(Color::Black),
      selectedRow(-1),
      selectedCol(-1),
      promotionColumn(-1) {

    window.create(sf::VideoMode({720, 750}), "Chess Titans", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    renderer = std::make_unique<Renderer>(window, 80);
    board.setupInitialPosition();
    ai = std::make_unique<AI>(aiColor, 4);
    renderer->loadFont("C:/Windows/Fonts/seguisym.ttf");
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void Game::handleEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseEvent->button == sf::Mouse::Button::Left) {
                if (state == GameState::Promotion) {
                    handlePromotion(mouseEvent->position.x, mouseEvent->position.y);
                } else if (state == GameState::PlayerTurn) {
                    handleMouseClick(mouseEvent->position.x, mouseEvent->position.y);
                }
            }
        }

        // Keyboard shortcuts
        if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
            if (keyEvent->code == sf::Keyboard::Key::Escape) {
                deselectPiece();
            }
            if (keyEvent->code == sf::Keyboard::Key::R) {
                // Restart game
                board.setupInitialPosition();
                state = GameState::PlayerTurn;
                deselectPiece();
                lastMove = Move();
            }
        }
    }
}

void Game::handleMouseClick(int x, int y) {
    int row, col;
    if (!renderer->getSquareFromMouse(x, y, row, col)) {
        deselectPiece();
        return;
    }

    Piece* clickedPiece = board.getPiece(row, col);

    // If no piece is selected
    if (selectedRow < 0) {
        // Select a piece if it belongs to the player
        if (clickedPiece && clickedPiece->getColor() == playerColor) {
            selectPiece(row, col);
        }
    } else {
        // A piece is already selected
        if (row == selectedRow && col == selectedCol) {
            // Clicked on the same piece - deselect
            deselectPiece();
        } else if (clickedPiece && clickedPiece->getColor() == playerColor) {
            // Clicked on another player piece - select it instead
            selectPiece(row, col);
        } else {
            // Try to move to the clicked square
            tryMove(row, col);
        }
    }
}

void Game::handlePromotion(int x, int y) {
    int row, col;
    if (!renderer->getSquareFromMouse(x, y, row, col)) {
        return;
    }

    // Check if click is in the promotion column
    if (col != promotionColumn) {
        return;
    }

    // Determine which piece was selected based on row
    int startRow = (playerColor == Color::White) ? 0 : 4;
    int relativeRow = row - startRow;

    if (relativeRow >= 0 && relativeRow < 4) {
        PieceType promotionTypes[] = {PieceType::Queen, PieceType::Rook,
                                       PieceType::Bishop, PieceType::Knight};
        pendingPromotionMove.promotionPiece = promotionTypes[relativeRow];

        // Make the promotion move
        board.makeMove(pendingPromotionMove);
        lastMove = pendingPromotionMove;
        deselectPiece();

        checkGameEnd();
        if (state == GameState::PlayerTurn) {
            state = GameState::AIThinking;
        }
    }
}

void Game::selectPiece(int row, int col) {
    selectedRow = row;
    selectedCol = col;
    currentLegalMoves = board.getLegalMovesForPiece(row, col);
}

void Game::deselectPiece() {
    selectedRow = -1;
    selectedCol = -1;
    currentLegalMoves.clear();
    promotionColumn = -1;
}

void Game::tryMove(int row, int col) {
    // Find a matching legal move
    for (const auto& move : currentLegalMoves) {
        if (move.toRow == row && move.toCol == col) {
            // Check if it's a promotion move
            if (move.isPromotion()) {
                pendingPromotionMove = move;
                promotionColumn = col;
                state = GameState::Promotion;
                return;
            }

            makePlayerMove(move);
            return;
        }
    }

    // No valid move - deselect
    deselectPiece();
}

void Game::makePlayerMove(const Move& move) {
    board.makeMove(move);
    lastMove = move;
    deselectPiece();

    checkGameEnd();
    if (state == GameState::PlayerTurn) {
        state = GameState::AIThinking;
    }
}

void Game::makeAIMove() {
    Move aiMove = ai->getBestMove(board);

    if (aiMove.isValid()) {
        board.makeMove(aiMove);
        lastMove = aiMove;
    }

    checkGameEnd();
    if (state == GameState::AIThinking) {
        state = GameState::PlayerTurn;
    }
}

void Game::checkGameEnd() {
    Color currentColor = board.getCurrentTurn();

    if (board.isCheckmate(currentColor)) {
        state = GameState::Checkmate;
    } else if (board.isStalemate(currentColor)) {
        state = GameState::Stalemate;
    } else if (board.isDraw()) {
        state = GameState::Draw;
    }
}

std::string Game::getStatusText() const {
    switch (state) {
        case GameState::PlayerTurn:
            if (board.isInCheck(playerColor)) {
                return "Your turn - CHECK!";
            }
            return "Your turn (White)";

        case GameState::AIThinking:
            return "Computer is thinking...";

        case GameState::Promotion:
            return "Select promotion piece";

        case GameState::Checkmate:
            if (board.getCurrentTurn() == playerColor) {
                return "Checkmate! You lose. Press R to restart.";
            } else {
                return "Checkmate! You win! Press R to restart.";
            }

        case GameState::Stalemate:
            return "Stalemate! Draw. Press R to restart.";

        case GameState::Draw:
            return "Draw! Press R to restart.";

        default:
            return "";
    }
}

void Game::update() {
    if (state == GameState::AIThinking) {
        // Make AI move (in a real game, this might be threaded)
        makeAIMove();
    }
}

void Game::render() {
    window.clear(sf::Color(50, 50, 50));

    renderer->render(board, selectedRow, selectedCol, currentLegalMoves, lastMove);

    if (state == GameState::Promotion) {
        renderer->drawPromotionDialog(playerColor, promotionColumn);
    }

    renderer->drawStatus(getStatusText());

    window.display();
}
