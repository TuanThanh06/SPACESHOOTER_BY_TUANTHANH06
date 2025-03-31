#include "texture.h"
#include <iostream>
SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cout << "Unable to load image: " << path << " | Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    return texture;
}
