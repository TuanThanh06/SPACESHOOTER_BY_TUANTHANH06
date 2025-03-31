#ifndef SDLUTILS_H_INCLUDED
#define SDLUTILS_H_INCLUDED
#include <SDL.h>
#include <iostream>
#include <string>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
// Khởi tạo SDL, cửa sổ, và renderer
bool initSDL(SDL_Window*& window, SDL_Renderer*& renderer, int width, int height);
// Dọn dẹp tài nguyên SDL
void cleanSDL(SDL_Window* window, SDL_Renderer* renderer);
// Hàm tải hiệu ứng âm thanh
Mix_Chunk* loadSound(const std::string& filePath);
// Hàm tải nhạc nền
Mix_Music* loadMusic(const std::string& filePath);
#endif // SDLUTILS_H_INCLUDED
