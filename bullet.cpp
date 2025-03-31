#include "bullet.h"
#include <cmath>
#include "texture.h"
#include <iostream>
// Khởi tạo danh sách đạn
void initBullets(Bullet bullets[], int maxBullets) {
    for (int i = 0; i < maxBullets; i++) {
        bullets[i].active = false; // Ban đầu tất cả đạn đều không hoạt động
        bullets[i].texture = nullptr; // Đặt texture ban đầu là nullptr
        bullets[i].isEnemyBullet = false; // Đặt mặc định là đạn của phi thuyền
    }
}
// Tạo đạn mới
void spawnBullet(Bullet bullets[], int maxBullets, int x, int y, float dx, float dy, float speed, SDL_Renderer* renderer, bool isEnemyBullet) {
    float magnitude = sqrt(dx * dx + dy * dy);
    if (magnitude != 0) {
        dx /= magnitude; // Chuẩn hóa vector hướng (đảm bảo chiều dài = 1)
        dy /= magnitude;
    }
    for (int i = 0; i < maxBullets; i++) {
        if (!bullets[i].active) {
            // Khởi tạo thông số cơ bản của đạn
            bullets[i].rect = {x, y, 20, 20};
            bullets[i].realX = x;
            bullets[i].realY = y;
            bullets[i].dx = dx;
            bullets[i].dy = dy;
            bullets[i].speed = speed;
            bullets[i].active = true;
            bullets[i].isEnemyBullet = isEnemyBullet; // Đặt loại đạn
            // Gán texture dựa trên loại đạn
            if (isEnemyBullet) {
                bullets[i].texture = loadTexture("assets/enemy_bullet.png", renderer);
            } else {
                bullets[i].texture = loadTexture("assets/player_bullet.png", renderer);
            }
            break; // Chỉ spawn một viên đạn
        }
    }
}
// Cập nhật vị trí của đạn
void updateBullets(Bullet bullets[], int maxBullets) {
    for (int i = 0; i < maxBullets; i++) {
        if (bullets[i].active) {
            bullets[i].realX += bullets[i].dx * bullets[i].speed; // Cập nhật vị trí thực X
            bullets[i].realY += bullets[i].dy * bullets[i].speed; // Cập nhật vị trí thực Y
            bullets[i].rect.x = static_cast<int>(bullets[i].realX); // Đồng bộ vị trí hiển thị X
            bullets[i].rect.y = static_cast<int>(bullets[i].realY); // Đồng bộ vị trí hiển thị Y
            // Vô hiệu hóa đạn nếu ra ngoài màn hình
            if (bullets[i].rect.x < 0 || bullets[i].rect.x > 800 || bullets[i].rect.y < 0 || bullets[i].rect.y > 600) {
                bullets[i].active = false;
                // Dọn dẹp texture để tránh rò rỉ bộ nhớ
                if (bullets[i].texture) {
                    SDL_DestroyTexture(bullets[i].texture);
                    bullets[i].texture = nullptr;
                }
            }
        }
    }
}
// Render đạn
void renderBullets(SDL_Renderer* renderer, Bullet bullets[], int maxBullets) {
    for (int i = 0; i < maxBullets; i++) {
        if (bullets[i].active) {
            if (bullets[i].texture) {
                // Vẽ texture của đạn
                SDL_RenderCopy(renderer, bullets[i].texture, NULL, &bullets[i].rect);
            }
        }
    }
}
// Dọn dẹp tài nguyên của đạn
void cleanBullets(Bullet bullets[], int maxBullets) {
    for (int i = 0; i < maxBullets; i++) {
        if (bullets[i].texture) {
            SDL_DestroyTexture(bullets[i].texture);
            bullets[i].texture = nullptr;
        }
    }
}
