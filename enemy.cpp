#include "enemy.h"
#include "bullet.h"
#include "texture.h"
#include <iostream>
#include <SDL_mixer.h>
const char* enemyTexturePaths[] = {
    "assets/enemy1.png",
    "assets/enemy2.png",
    "assets/enemy3.png",
    "assets/enemy4.png"
};
const int NUM_ENEMY_TEXTURES = sizeof(enemyTexturePaths) / sizeof(enemyTexturePaths[0]);
// Kẻ địch bắn đạn tản
void enemyShoot(Bullet bullets[], int maxBullets, const Enemy& enemy, SDL_Renderer* renderer) {
    // Đảm bảo đạn được đánh dấu là của kẻ địch
    spawnBullet(bullets, maxBullets, enemy.rect.x + enemy.rect.w / 2, enemy.rect.y, 0, 1, 1.0, renderer, true);  // Đạn thẳng xuống
    spawnBullet(bullets, maxBullets, enemy.rect.x + enemy.rect.w / 2, enemy.rect.y, -0.5, 1, 1.0, renderer, true); // Đạn lệch trái
    spawnBullet(bullets, maxBullets, enemy.rect.x + enemy.rect.w / 2, enemy.rect.y, 0.5, 1, 1.0, renderer, true);  // Đạn lệch phải
}
// Khởi tạo mảng kẻ địch
void initEnemies(Enemy enemies[], int maxEnemies) {
    for (int i = 0; i < maxEnemies; i++) {
        enemies[i].active = false; // Ban đầu tất cả kẻ địch đều không hoạt động
        enemies[i].texture = nullptr; // Đặt texture là nullptr
    }
}
// Tạo kẻ địch mới
void spawnEnemy(Enemy enemies[], int maxEnemies, int x, int y, int width, int height, float speed, SDL_Renderer* renderer) {
    for (int i = 0; i < maxEnemies; i++) {
        if (!enemies[i].active) {
            enemies[i].rect = {x, y, width, height};
            enemies[i].realY = y; // Vị trí thực
            enemies[i].speed = speed;
            enemies[i].active = true;
            // Chọn texture ngẫu nhiên
            int randomIndex = rand() % NUM_ENEMY_TEXTURES;
            enemies[i].texture = loadTexture(enemyTexturePaths[randomIndex], renderer);
            break; // Tạo duy nhất một kẻ địch
        }
    }
}
// Cập nhật trạng thái kẻ địch và xử lý bắn đạn
void updateEnemiesAndShoot(Bullet bullets[], int maxBullets, Enemy enemies[], int maxEnemies, SDL_Renderer* renderer, Mix_Chunk* shootSound,SDL_Texture* enemyBulletTexture) {
    for (int i = 0; i < maxEnemies; i++) {
        if (enemies[i].active) {
            // Di chuyển kẻ địch
            enemies[i].realY += enemies[i].speed; // Cập nhật vị trí thực
            enemies[i].rect.y = static_cast<int>(enemies[i].realY); // Đồng bộ với SDL_Rect

            // Xác suất kẻ địch bắn đạn ngẫu nhiên (0.01% mỗi khung hình)
            if (rand() % 10000 < 1) {
                enemyShoot(bullets, maxBullets, enemies[i], renderer); // Hàm bắn đạn
                Mix_PlayChannel(-1, shootSound, 0); // Phát âm thanh bắn
            }
            // Vô hiệu hóa kẻ địch nếu ra khỏi màn hình
            if (enemies[i].rect.y > 600) {
                enemies[i].active = false;
                // Giải phóng texture của kẻ địch
                if (enemies[i].texture) {
                    SDL_DestroyTexture(enemies[i].texture);
                    enemies[i].texture = enemyBulletTexture;
                }
            }
        }
    }
}
// Render kẻ địch
void renderEnemies(SDL_Renderer* renderer, Enemy enemies[], int maxEnemies) {
    for (int i = 0; i < maxEnemies; i++) {
        if (enemies[i].active) {
            if (enemies[i].texture) {
                // Render texture của kẻ địch
                SDL_RenderCopy(renderer, enemies[i].texture, NULL, &enemies[i].rect);
            }
        }
    }
}
