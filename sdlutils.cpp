#include "SDLUtils.h"
#include <SDL_image.h>
bool initSDL(SDL_Window*& window, SDL_Renderer*& renderer, int width, int height) {
    // Khởi tạo SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL khong the khoi tao: " << SDL_GetError() << std::endl;
        return false;
    }
    // Khởi tạo SDL_image
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cout << "SDL_image khong the khoi tao: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return false;
    }
    // Tạo cửa sổ
    window = SDL_CreateWindow("SPACE SHOOTER", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "Khong the khoi tao cua so: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return false;
    }
    // Tạo renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "Khong the khoi tao renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return false;
    }
    // Tạo ttf
    if (TTF_Init() < 0) {
        std::cout << "Khong the khoi tao SDL_ttf: " << TTF_GetError() << std::endl;
        return -1;
    }
    // Khởi tạo mmixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "Khong the khoi tao SDL_Mixer: " << Mix_GetError() << std::endl;
        return -1;
    }
    return true; // Thành công
}
void cleanSDL(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}
Mix_Chunk* loadSound(const std::string& filePath) {
    Mix_Chunk* sound = Mix_LoadWAV(filePath.c_str());
    if (!sound) {
        std::cout << "Khong the tai am thanh: " << Mix_GetError() << std::endl;
    }
    return sound;
}
Mix_Music* loadMusic(const std::string& filePath) {
    Mix_Music* music = Mix_LoadMUS(filePath.c_str());
    if (!music) {
        std::cout << "Khong the tai nhac nen: " << Mix_GetError() << std::endl;
    }
    return music;
}
