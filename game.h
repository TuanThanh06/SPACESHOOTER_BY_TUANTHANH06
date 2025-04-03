#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include <SDL.h>
#include <iostream>
#include "bullet.h"
#include <SDL_ttf.h>
#include <string>
#include <SDL_mixer.h>
#include <fstream>
// Cấu trúc phi thuyền
struct Ship {
    SDL_Rect rect;          // Vị trí và kích thước phi thuyền
    SDL_Texture* texture;   // Texture của phi thuyền
    int hearts[3];          // Trạng thái của trái tim (0: mất, 1: nửa, 2: đầy)
    bool isBlinking;        // Cờ kiểm tra trạng thái nhấp nháy
    int blinkCounter;       // Bộ đếm thời gian nhấp nháy
};
// Tạo phi thuyền
Ship createShip(int x, int y, int width, int height, SDL_Renderer* renderer);
// Xử lý sự kiện chuột và thoát game
void handleEvents(bool& running, Ship& ship, Bullet bullets[], int maxBullets, SDL_Renderer* renderer, Mix_Chunk* shootSound);
// Hàm render phi thuyền
void renderShip(SDL_Renderer* renderer, const Ship& ship, SDL_Texture* fullHeartTexture, SDL_Texture* halfHeartTexture);
// Xử lý va chạm giữa đạn và phi thuyền
void handleBulletShipCollision(Bullet bullets[], int maxBullets, Ship& ship);
// Cấu trúc để quản lý điểm số
struct Score {
    int currentScore;       // Điểm số hiện tại
    TTF_Font* font;         // Font chữ để hiển thị
};
// Khởi tạo điểm số
Score createScore(const std::string& fontPath, int fontSize);
// Cập nhật điểm số
void updateScore(Score& score, int increment);
// Hiển thị điểm số trên màn hình
void renderScore(SDL_Renderer* renderer, const Score& score);
// Hiển thị Game Over và điểm số
void showGameOver(SDL_Renderer* renderer,Score& score);
void saveHighScore(const std::string& filename, int highScore);
int loadHighScore(const std::string& filename);
#endif // GAME_H_INCLUDED
