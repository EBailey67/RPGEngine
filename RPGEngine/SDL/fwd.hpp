#pragma once

#include "../fwd.hpp"
#include <SDL_mixer.h>
#include <SDL_render.h>
#include <SDL_ttf.h>

class BasicScene;
class GameEngine;
class TileSet;
class Game;
class Instances;

// Resources cache for textures.
using TextureCache = cache<SDL_Texture *, SDL_DestroyTexture>;

// Resources cache for fonts.
using FontCache = cache<TTF_Font *, TTF_CloseFont>;

// Resources cache for audio.
using MusicCache = cache<Mix_Music *, Mix_FreeMusic>;

