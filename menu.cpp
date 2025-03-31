#include "menu.h"
#include <iostream>
#include <string>
#include "texture.h"
#include "game.h"
// Hàm render text với màu sắc
SDL_Texture* renderTextWithColor(const std::string& text, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}
// Hàm hiển thị menu chính
int showMenu(SDL_Renderer* renderer, Mix_Chunk* clickSound, Mix_Chunk* hoverSound) {
    // Tải high score từ file
    int highScore = loadHighScore("highscore.txt");
    // Xóa renderer trước khi hiển thị menu
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Màu nền đen
    SDL_RenderClear(renderer);
    SDL_Texture* bgTexture = loadTexture("assets/background_menu.png", renderer);
    if (!bgTexture) {
        std::cerr << "khong the tai background menu!" << std::endl;
        return -1; // Thoát nếu không tải được hình nền
    }
    Mix_Music* backgroundMusic = Mix_LoadMUS("assets/menu_music.mp3");
    if (!backgroundMusic) {
        std::cerr << "khong the tai nhac nen: " << Mix_GetError() << std::endl;
        return -1;
    }
    TTF_Font* font = TTF_OpenFont("assets/font.ttf", 25);
    if (!font) {
        std::cerr << "khong the tai font: " << TTF_GetError() << std::endl;
        return -1;
    }
    SDL_Color normalColor = {255, 255, 255};
    SDL_Color hoverColor = {255, 255, 0};
    // Tạo texture cho các mục menu
    SDL_Texture* startGameText = renderTextWithColor("1. Start Game", font, normalColor, renderer);
    SDL_Texture* instructionsText = renderTextWithColor("2. Instruction", font, normalColor, renderer);
    SDL_Texture* exitText = renderTextWithColor("3. Exit", font, normalColor, renderer);
    // Tạo texture cho high score
    SDL_Texture* highScoreText = renderTextWithColor(
        "High Score: " + std::to_string(highScore), font, normalColor, renderer);
    SDL_Rect highScoreRect = {300, 100, 300, 50}; // Vị trí hiển thị high score
    SDL_Rect startGameRect = {300, 350, 200, 50};
    SDL_Rect instructionsRect = {300, 400, 200, 50};
    SDL_Rect exitRect = {300, 450, 100, 50};
    bool running = true;
    int selectedOption = -1;
    int previousHover = -1; // Theo dõi mục đã được hover trước đó
    Mix_PlayMusic(backgroundMusic, -1);
    while (running) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        // Render background menu
        SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        int currentHover = -1; // Theo dõi mục hiện tại chuột đang hover
        // Xác định mục chuột đang hover
        if (mouseX >= startGameRect.x && mouseX <= startGameRect.x + startGameRect.w &&
            mouseY >= startGameRect.y && mouseY <= startGameRect.y + startGameRect.h) {
            currentHover = 1; // Hover Start Game
        } else if (mouseX >= instructionsRect.x && mouseX <= instructionsRect.x + instructionsRect.w &&
                   mouseY >= instructionsRect.y && mouseY <= instructionsRect.y + instructionsRect.h) {
            currentHover = 2; // Hover Instructions
        } else if (mouseX >= exitRect.x && mouseX <= exitRect.x + exitRect.w &&
                   mouseY >= exitRect.y && mouseY <= exitRect.y + exitRect.h) {
            currentHover = 3; // Hover Exit
        }
        // Phát âm thanh hover nếu chuột di vào mục mới
        if (currentHover != previousHover && currentHover != -1) {
            Mix_PlayChannel(-1, hoverSound, 0); // Phát âm thanh hover
            previousHover = currentHover;      // Cập nhật trạng thái hover
        }
        // Cập nhật màu sắc của mục menu dựa trên trạng thái hover
        SDL_Color startColor = (currentHover == 1) ? hoverColor : normalColor;
        SDL_Color instructionsColor = (currentHover == 2) ? hoverColor : normalColor;
        SDL_Color exitColor = (currentHover == 3) ? hoverColor : normalColor;
        startGameText = renderTextWithColor("1. Start Game", font, startColor, renderer);
        instructionsText = renderTextWithColor("2. Instructions", font, instructionsColor, renderer);
        exitText = renderTextWithColor("3. Exit", font, exitColor, renderer);
        // Render high score và các mục menu
        SDL_RenderCopy(renderer, highScoreText, NULL, &highScoreRect); // High Score
        SDL_RenderCopy(renderer, startGameText, NULL, &startGameRect); // Start Game
        SDL_RenderCopy(renderer, instructionsText, NULL, &instructionsRect); // Instructions
        SDL_RenderCopy(renderer, exitText, NULL, &exitRect); // Exit
        SDL_RenderPresent(renderer);
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (currentHover == 1) {
                    Mix_PlayChannel(-1, clickSound, 0); // Phát âm thanh click
                    selectedOption = 1;
                    running = false;
                } else if (currentHover == 2) {
                    Mix_PlayChannel(-1, clickSound, 0); // Phát âm thanh click
                    selectedOption = 2;
                    running = false;
                } else if (currentHover == 3) {
                    Mix_PlayChannel(-1, clickSound, 0); // Phát âm thanh click
                    selectedOption = 3;
                    running = false;
                }
            }
        }
        // Dọn dẹp texture mục menu (tránh rò rỉ bộ nhớ)
        SDL_DestroyTexture(startGameText);
        SDL_DestroyTexture(instructionsText);
        SDL_DestroyTexture(exitText);
    }
    // Dọn dẹp tài nguyên menu
    SDL_DestroyTexture(highScoreText);
    SDL_DestroyTexture(bgTexture);
    Mix_FreeMusic(backgroundMusic);
    TTF_CloseFont(font);
    return selectedOption;
}
