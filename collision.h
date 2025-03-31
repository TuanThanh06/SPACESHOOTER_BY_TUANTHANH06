#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED
#include "SDL.h"
#include "bullet.h"
#include "enemy.h"
#include "game.h"
#include <SDL_mixer.h>
// Cấu trúc lưu trữ hiệu ứng hạ
struct Explosion {
    SDL_Rect rect;          // Vị trí và kích thước hiệu ứng hạ
    SDL_Texture* texture;   // Texture của hiệu ứng hạ
    int currentFrame;       // Khung hình hiện tại
    int totalFrames;        // Tổng số khung hình trong sprite sheet
    int frameDelay;         // Thời gian giữa các khung hình
    int frameCounter;       // Bộ đếm để chuyển đổi khung hình
    bool active;            // Trạng thái hoạt động
};
void handleBulletShipCollision(Bullet bullets[], int maxBullets, Ship& ship,Mix_Chunk* hitSound) ;
// Kiểm tra va chạm giữa hai SDL_Rect
bool checkCollision(const SDL_Rect& a, const SDL_Rect& b);
// Xử lý va chạm giữa đạn và kẻ địch
void handleBulletEnemyCollision(Bullet bullets[], int maxBullets, Enemy enemies[], int maxEnemies, Explosion explosions[], int maxExplosions, SDL_Texture* explosionTexture,Score& score, Mix_Chunk* explosionSound);
// Kiểm tra va chạm giữa phi thuyền và kẻ địch
bool checkShipEnemyCollision(const Ship& ship, Enemy enemies[], int maxEnemies);
// Render hiệu ứng nổ
void renderExplosions(SDL_Renderer* renderer, Explosion explosions[], int maxExplosions);
// Dọn dẹp tài nguyên nổ
void cleanExplosions(Explosion explosions[], int maxExplosions);
#endif // COLLISION_H_INCLUDED
