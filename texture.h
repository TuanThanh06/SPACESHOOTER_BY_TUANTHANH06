#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED
#include <SDL.h>
#include <SDL_image.h>
#include <string>
SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer);
#endif // TEXTURE_H_INCLUDED
