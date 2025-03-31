#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <SDL_mixer.h>
// Hiển thị menu chính và trả về tùy chọn người chơi
int showMenu(SDL_Renderer* renderer, Mix_Chunk* clickSound, Mix_Chunk* hoverSound);
// Hàm render text với màu sắc tùy chỉnh
SDL_Texture* renderTextWithColor(const std::string& text, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer);
#endif // MENU_H_INCLUDED
