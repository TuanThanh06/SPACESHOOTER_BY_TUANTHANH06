#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED
#include <SDL.h>
#include "bullet.h"
#include <SDL_mixer.h>
struct Enemy {
    SDL_Rect rect;          // Vị trí và kích thước của kẻ địch
    SDL_Texture* texture;   // Texture của kẻ địch
    float realY;            // Vị trí thực của kẻ địch (hỗ trợ số thập phân)
    float speed;            // Tốc độ di chuyển của kẻ địch
    bool active;            // Trạng thái kẻ địch (true: đang hoạt động, false: không hoạt động)
};
void enemyShoot(Bullet bullets[], int maxBullets, const Enemy& enemy, SDL_Renderer* renderer);
void initEnemies(Enemy enemies[], int maxEnemies);
// Sinh một kẻ địch mới tại
void spawnEnemy(Enemy enemies[], int maxEnemies, int x, int y, int width, int height, float speed, SDL_Renderer* renderer);
// Cập nhật trạng thái của kẻ địch: di chuyển xuống màn hình và bắn đạn theo xác suất.
void updateEnemiesAndShoot(Bullet bullets[], int maxBullets, Enemy enemies[], int maxEnemies, SDL_Renderer* renderer, Mix_Chunk* shootSound,SDL_Texture* enemyBulletTexture);
// Render (vẽ) kẻ địch lên màn hình
void renderEnemies(SDL_Renderer* renderer, Enemy enemies[], int maxEnemies);
#endif // ENEMY_H_INCLUDED
