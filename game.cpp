#include "game.h"
#include <iostream>
#include "texture.h"
#include <SDL_ttf.h>
Ship createShip(int x, int y, int width, int height, SDL_Renderer* renderer) {
    Ship ship;
    ship.rect = {x, y, width, height};
    ship.texture = loadTexture("assets/ship.png", renderer);
    ship.hearts[0] = 2;
    ship.hearts[1] = 2;
    ship.hearts[2] = 2;
    return ship;
}
void handleEvents(bool& running, Ship& ship, Bullet bullets[], int maxBullets, SDL_Renderer* renderer, Mix_Chunk* shootSound) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false; // Thoát game
        }
        if (event.type == SDL_MOUSEMOTION) {
            // Di chuyển phi thuyền theo chuột
            ship.rect.x = event.motion.x - ship.rect.w / 2;
            ship.rect.y = event.motion.y - ship.rect.h / 2;
        }
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            // Bắn đạn khi nhấn chuột trái
            spawnBullet(bullets, maxBullets, ship.rect.x + ship.rect.w / 2 - 5, ship.rect.y, 0, -1, 3.0, renderer, false);
            Mix_PlayChannel(-1, shootSound, 0);
        }
    }
}
void renderShip(SDL_Renderer* renderer, const Ship& ship, SDL_Texture* fullHeartTexture, SDL_Texture* halfHeartTexture) {
    // Quản lý hiệu ứng nhấp nháy
    if (ship.isBlinking) {
        const_cast<Ship&>(ship).blinkCounter++;
        if (ship.blinkCounter % 20 < 10) {
            // Ẩn phi thuyền trong khoảng thời gian nhấp nháy
            return;
        }
        if (ship.blinkCounter > 120) {
            // Kết thúc hiệu ứng nhấp nháy sau 120 khung hình
            const_cast<Ship&>(ship).isBlinking = false;
            const_cast<Ship&>(ship).blinkCounter = 0;
        }
    }
    // Vẽ phi thuyền
    if (ship.texture) {
        SDL_RenderCopy(renderer, ship.texture, NULL, &ship.rect);
    }
    // Vẽ các trái tim
    for (int i = 0; i < 3; i++) {
        SDL_Rect heartRect = {10 + i * 40, 600 - 40, 30, 30};
        if (ship.hearts[i] == 2) { // Trái tim đầy
            SDL_RenderCopy(renderer, fullHeartTexture, NULL, &heartRect);
        } else if (ship.hearts[i] == 1) { // Nửa trái tim
            SDL_RenderCopy(renderer, halfHeartTexture, NULL, &heartRect);
        }
    }
}
Score createScore(const std::string& fontPath, int fontSize) {
    Score score;
    score.currentScore = 0;
    score.font = TTF_OpenFont(fontPath.c_str(), fontSize);
    return score;
}
void updateScore(Score& score, int increment) {
    score.currentScore += increment; // Tăng điểm
}
void renderScore(SDL_Renderer* renderer, const Score& score) {
    SDL_Color textColor = {255, 255, 255}; // Màu chữ (trắng)
    std::string scoreText = "Score: " + std::to_string(score.currentScore);
    SDL_Surface* textSurface = TTF_RenderText_Solid(score.font, scoreText.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {20, 20, textSurface->w, textSurface->h};
    SDL_FreeSurface(textSurface);
    if (textTexture) {
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture(textTexture);
    }
}
void cleanScore(Score& score) {
    if (score.font) {
        TTF_CloseFont(score.font);
        score.font = nullptr;
    }
}
void showGameOver(SDL_Renderer* renderer, Score& score) {
    bool waiting = true;
    SDL_Event e;
    SDL_Texture* gameOverBg = loadTexture("assets/gameover_bg.png", renderer);
    while (waiting) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Màu nền đen
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, gameOverBg, NULL, NULL);
        // Hiển thị chữ "Game Over" và điểm số
        SDL_Color white = {255, 255, 255};
        TTF_Font* font = TTF_OpenFont("assets/font.ttf", 50);
        if (font) {
            // Render chữ "Game Over"
            SDL_Surface* surface = TTF_RenderText_Solid(font, "GAME OVER", white);
            SDL_Texture* gameOverText = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
            SDL_Rect gameOverRect = {200, 200, 400, 100};
            SDL_RenderCopy(renderer, gameOverText, NULL, &gameOverRect);
            SDL_DestroyTexture(gameOverText);
            // Render điểm số
            std::string scoreText = "Score: " + std::to_string(score.currentScore);
            surface = TTF_RenderText_Solid(font, scoreText.c_str(), white);
            SDL_Texture* scoreTextTexture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
            SDL_Rect scoreRect = {200, 300, 400, 100};
            SDL_RenderCopy(renderer, scoreTextTexture, NULL, &scoreRect);
            SDL_DestroyTexture(scoreTextTexture);
            TTF_CloseFont(font);
        }
        SDL_RenderPresent(renderer); // Hiển thị mọi thứ lên màn hình

        // Kiểm tra sự kiện nhấn chuột
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                waiting = false; // Quay lại menu
            }
        }
    }
}
void saveHighScore(const std::string& filename, int highScore) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << highScore; // Ghi điểm cao nhất vào file
        file.close();
    } else {
        std::cerr << "khong the mo file lưu high score!" << std::endl;
    }
}
int loadHighScore(const std::string& filename) {
    int highScore = 0;
    std::ifstream file(filename);
    if (file.is_open()) {
        file >> highScore; // Đọc điểm cao nhất từ file
        file.close();
    } else {
        std::cerr << "khong the mo file load high score! Sử dụng mặc định là 0." << std::endl;
    }
    return highScore;
}

