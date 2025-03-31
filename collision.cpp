#include "collision.h"
#include <iostream>
#include <SDL_mixer.h>
// Kiểm tra va chạm giữa hai hình chữ nhật (SDL_Rect)
bool checkCollision(const SDL_Rect& a, const SDL_Rect& b) {
    if (a.x + a.w <= b.x || // A nằm bên trái B
        a.x >= b.x + b.w || // A nằm bên phải B
        a.y + a.h <= b.y || // A nằm bên trên B
        a.y >= b.y + b.h) { // A nằm bên dưới B
        return false;       // Không có va chạm
    }
    return true; // Có va chạm
}
// Tạo hiệu ứng nổ
Explosion createExplosion(int x, int y, int width, int height, SDL_Texture* texture, int totalFrames, int frameDelay) {
    Explosion explosion;
    explosion.rect = {x, y, width, height};
    explosion.texture = texture;
    explosion.currentFrame = 0;
    explosion.totalFrames = totalFrames;
    explosion.frameDelay = frameDelay;
    explosion.frameCounter = 0;
    explosion.active = true;
    return explosion;
}
void handleBulletShipCollision(Bullet bullets[], int maxBullets, Ship& ship,Mix_Chunk* hitSound) {
    for (int i = 0; i < maxBullets; i++) {
        if (bullets[i].active && bullets[i].isEnemyBullet && checkCollision(bullets[i].rect, ship.rect)) {
            bullets[i].active = false;
            // Phát âm thanh khi phi thuyền bị bắn
            Mix_PlayChannel(-1, hitSound, 0);
            // Trừ mạng sống
            for (int j = 2; j >= 0; j--) {
                if (ship.hearts[j] == 2) {
                    ship.hearts[j] = 1; // Chuyển trái tim đầy thành nửa
                    break;
                } else if (ship.hearts[j] == 1) {
                    ship.hearts[j] = 0; // Xóa trái tim
                    break;
                }
            }
            // Kích hoạt nhấp nháy
            ship.isBlinking = true;
            ship.blinkCounter = 0; // Đặt lại bộ đếm nhấp nháy
            break;
        }
    }
}
// Xử lý va chạm giữa đạn và kẻ địch
void handleBulletEnemyCollision(Bullet bullets[], int maxBullets, Enemy enemies[], int maxEnemies, Explosion explosions[], int maxExplosions, SDL_Texture* explosionTexture, Score& score, Mix_Chunk* explosionSound) { // Thêm tham số `Score&`
    for (int i = 0; i < maxBullets; i++) {
        if (bullets[i].active && !bullets[i].isEnemyBullet) { // Chỉ xử lý đạn của phi thuyền
            for (int j = 0; j < maxEnemies; j++) {
                if (enemies[j].active && checkCollision(bullets[i].rect, enemies[j].rect)) {
                    // Vô hiệu hóa đạn và kẻ địch
                    bullets[i].active = false;
                    enemies[j].active = false;
                    // Phát âm thanh nổ
                    Mix_PlayChannel(-1, explosionSound, 0);
                    // Tạo hiệu ứng nổ tại vị trí kẻ địch
                    for (int k = 0; k < maxExplosions; k++) {
                        if (!explosions[k].active) {
                            explosions[k] = createExplosion(
                                enemies[j].rect.x,
                                enemies[j].rect.y,
                                100, // Kích thước khung hình nổ
                                100, // Chiều cao khung hình nổ
                                explosionTexture,
                                8,  // Số khung hình trong sprite sheet
                                10   // Độ trễ giữa các khung hình
                            );
                            break;
                        }
                    }
                    // **Cập nhật điểm số**
                    updateScore(score, 10); // Tăng 10 điểm khi tiêu diệt một kẻ địch
                    // Giải phóng texture của kẻ địch nếu có
                    if (enemies[j].texture) {
                        SDL_DestroyTexture(enemies[j].texture);
                        enemies[j].texture = nullptr;
                    }
                    break;
                }
            }
        }
    }
}
// Kiểm tra va chạm giữa phi thuyền và kẻ địch
bool checkShipEnemyCollision(const Ship& ship, Enemy enemies[], int maxEnemies) {
    for (int i = 0; i < maxEnemies; i++) {
        if (enemies[i].active && checkCollision(ship.rect, enemies[i].rect)) {
            return true; // Va chạm xảy ra
        }
    }
    return false; // Không có va chạm
}
// Render hiệu ứng nổ
void renderExplosions(SDL_Renderer* renderer, Explosion explosions[], int maxExplosions) {
    for (int i = 0; i < maxExplosions; i++) {
        if (explosions[i].active) {
            int frameWidth = explosions[i].rect.w;
            int frameHeight = explosions[i].rect.h;
            SDL_Rect srcRect = {
                explosions[i].currentFrame * frameWidth, // Vị trí khung hình trong sprite sheet
                0,
                frameWidth,
                frameHeight
            };
            SDL_RenderCopy(renderer, explosions[i].texture, &srcRect, &explosions[i].rect);
            // Chuyển đổi khung hình
            explosions[i].frameCounter++;
            if (explosions[i].frameCounter >= explosions[i].frameDelay) {
                explosions[i].currentFrame++;
                explosions[i].frameCounter = 0;
                if (explosions[i].currentFrame >= explosions[i].totalFrames) {
                    explosions[i].active = false; // Hiệu ứng nổ kết thúc
                }
            }
        }
    }
}
// Dọn dẹp tài nguyên hiệu ứng nổ
void cleanExplosions(Explosion explosions[], int maxExplosions) {
    for (int i = 0; i < maxExplosions; i++) {
        if (explosions[i].texture) {
            SDL_DestroyTexture(explosions[i].texture);
            explosions[i].texture = nullptr;
        }
    }
}
