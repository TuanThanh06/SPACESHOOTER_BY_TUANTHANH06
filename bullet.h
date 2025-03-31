#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED
#include <SDL.h>
// Cấu trúc viên đạn
struct Bullet {
    SDL_Rect rect;         // Vị trí và kích thước viên đạn
    float realX;           // Vị trí thực (trục X)
    float realY;           // Vị trí thực (trục Y)
    float dx, dy;          // Hướng di chuyển (dx: ngang, dy: dọc)
    float speed;           // Tốc độ di chuyển của viên đạn
    bool active;           // Trạng thái của đạn (true: đang hoạt động, false: không hoạt động)
    bool isEnemyBullet;    // true nếu đạn là của kẻ địch
    SDL_Texture* texture;  // Texture của viên đạn
};
// Khởi tạo mảng đạn
void initBullets(Bullet bullets[], int maxBullets);
// Tạo một viên đạn mới
void spawnBullet(Bullet bullets[], int maxBullets, int x, int y, float dx, float dy, float speed, SDL_Renderer* renderer,bool isEnemyBullet);
// Cập nhật tất cả các viên đạn
void updateBullets(Bullet bullets[], int maxBullets);
// Vẽ tất cả các viên đạn
void renderBullets(SDL_Renderer* renderer, Bullet bullets[], int maxBullets);
// Dọn dẹp tài nguyên đạn
void cleanBullets(Bullet bullets[], int maxBullets);
#endif // BULLET_H_INCLUDED
