#include "SDLUtils.h"
#include "game.h"
#include <iostream>
#include "enemy.h"
#include "texture.h"
#include "collision.h"
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "menu.h"

using namespace std;

const int MAX_BULLETS = 100;
const int MAX_ENEMIES = 20;
const int MAX_EXPLOSIONS = 20;

int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    // Khởi tạo SDL
    initSDL(window, renderer, 800, 600);
    Mix_Chunk* clickSound = Mix_LoadWAV("assets/click.wav");
    Mix_Chunk* hoverSound = Mix_LoadWAV("assets/hover.wav");
    Mix_Music* backgroundMusic = Mix_LoadMUS("assets/background_music.mp3");
    int highScore = loadHighScore("highscore.txt");
    Score score = createScore("assets/font.ttf", 20);
    SDL_Texture* backgroundTexture = loadTexture("assets/background.png", renderer);
    SDL_Texture* instructionsBg = loadTexture("assets/instructions_bg.png", renderer);
    SDL_Texture* enemyBulletTexture = loadTexture("assets/enemy_bullet.png", renderer);
    SDL_Texture* fullHeartTexture = loadTexture("assets/full_heart.png", renderer);
    SDL_Texture* halfHeartTexture = loadTexture("assets/half_heart.png", renderer);
    Mix_Chunk* shootSound = loadSound("assets/shoot.wav");
    Mix_Chunk* hitSound = loadSound("assets/hit.wav");
    SDL_Texture* explosionTexture = loadTexture("assets/explosion.png", renderer);
    Mix_Chunk* explosionSound = loadSound("assets/explosion.wav");
    // Tạo phi thuyền
    Ship ship = createShip(800 / 2 - 25, 600 - 60, 50, 50, renderer);
    // Tạo danh sách các đạn, kẻ địch và hiệu ứng nổ
    Bullet bullets[MAX_BULLETS];
    initBullets(bullets, MAX_BULLETS);
    Enemy enemies[MAX_ENEMIES];
    initEnemies(enemies, MAX_ENEMIES);
    Explosion explosions[MAX_EXPLOSIONS] = {};
    bool isPlaying = true ;
    while (isPlaying) {
    int menuOption = showMenu(renderer,clickSound,hoverSound);
    if (menuOption == 1) {
        cout << "Start the game!" << endl;
        // Vòng lặp chính của game ở đây
        bool running = true;
        Mix_PlayMusic(backgroundMusic, -1);
        // Ẩn Chuột
        SDL_ShowCursor(SDL_DISABLE);
        while (running) {
            // Xử lý sự kiện
            handleEvents(running, ship, bullets, MAX_BULLETS, renderer, shootSound);
            handleBulletShipCollision(bullets, MAX_BULLETS, ship,hitSound);
            // Cập nhật logic đạn
            updateBullets(bullets, MAX_BULLETS);
            // Cập nhật logic của kẻ địch: di chuyển + bắn đạn ngẫu nhiên
            updateEnemiesAndShoot(bullets, MAX_BULLETS, enemies, MAX_ENEMIES, renderer, shootSound,enemyBulletTexture);
            // Xử lý va chạm giữa đạn và kẻ địch
            handleBulletEnemyCollision(bullets, MAX_BULLETS, enemies, MAX_ENEMIES, explosions, MAX_EXPLOSIONS, explosionTexture, score, explosionSound);
            // Kiểm tra va chạm giữa phi thuyền và kẻ địch
            if (checkShipEnemyCollision(ship, enemies, MAX_ENEMIES)) {
                // Duyệt trái tim từ cuối về đầu (phải → trái)
                for (int j = 2; j >= 0; j--) {
                    if (ship.hearts[j] == 2) { // Nếu là trái tim đầy
                        ship.hearts[j] = 1; // Chuyển thành nửa trái tim
                        break;
                    } else if (ship.hearts[j] == 1) { // Nếu là nửa trái tim
                        ship.hearts[j] = 0; // Xóa trái tim (biến mất)
                        break;
                    }
                }
                // Kích hoạt hiệu ứng nhấp nháy
                ship.isBlinking = true;
                ship.blinkCounter = 0; // Đặt lại bộ đếm nhấp nháy
            }
            // Kiểm tra nếu tất cả trái tim biến mất
            bool allGone = true; // Giả định tất cả trái tim đã biến mất
            for (int j = 0; j < 3; j++) {
                if (ship.hearts[j] != 0) { // Nếu còn bất kỳ trái tim nào
                    allGone = false;
                    break;
                }
            }
            // Spawn kẻ địch mới sau mỗi vài giây
            if (rand() % 1000 < 1.5) { // Xác suất sinh ra kẻ địch mới
                spawnEnemy(enemies, MAX_ENEMIES, rand() % 800, 0, 60, 60, 0.1, renderer);
            }
            // Render Background
            SDL_RenderCopy(renderer,backgroundTexture,NULL,NULL);
            // Render phi thuyền
            renderShip(renderer, ship, fullHeartTexture, halfHeartTexture);
            // Render các đối tượng khác
            renderBullets(renderer, bullets, MAX_BULLETS);
            renderEnemies(renderer, enemies, MAX_ENEMIES);
            renderExplosions(renderer, explosions, MAX_EXPLOSIONS); // Render hiệu ứng hạ
            // **Render điểm số**
            renderScore(renderer, score);
            SDL_RenderPresent(renderer); // Hiển thị frame
            if (allGone) {
                if (score.currentScore > highScore) {
                    highScore = score.currentScore; // Cập nhật điểm cao nhất
                    saveHighScore("highscore.txt", highScore); // Lưu điểm cao nhất vào file
                }
                running = false ; // Kết thúc vòng lặp game
                showGameOver(renderer, score);
                // Hiện chuột
                SDL_ShowCursor(SDL_ENABLE);
            }
        }
    } else if (menuOption == 2) {
        cout << "Game instructions!" << endl;
        Mix_PlayMusic(backgroundMusic, -1);
        SDL_RenderCopy(renderer, instructionsBg, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Event e;
        bool waiting = true;
        while (waiting) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN) {
                    waiting = false; // Thoát màn hình hướng dẫn và quay lại menu
                }
            }
        }
    } else if (menuOption == 3) {
        cout << "Exit the game!" << endl;
        isPlaying = false ;
        }
    }
    Mix_FreeChunk(clickSound);
    Mix_FreeChunk(hoverSound);
    Mix_FreeChunk(shootSound);
    Mix_FreeChunk(hitSound);
    Mix_FreeChunk(explosionSound);
    Mix_FreeMusic(backgroundMusic);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(enemyBulletTexture);
    SDL_DestroyTexture(fullHeartTexture);
    SDL_DestroyTexture(halfHeartTexture);
    SDL_DestroyTexture(explosionTexture);
    SDL_DestroyTexture(instructionsBg);
    cleanSDL(window,renderer);
    return 0;
}
