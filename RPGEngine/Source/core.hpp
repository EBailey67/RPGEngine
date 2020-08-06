#pragma once

#include <cstddef>
#include <filesystem>
#include <map>
#include <string>
#include <type_traits>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>

#include "component/components.hpp"
#include "config.hpp"
#include "entt/entt.hpp"
#include "resource/cache.hpp"
#include "sdl/sdl.hpp"
#include "Utility/Debug.h"


inline entt::registry registry{};
inline TextureCache textureCache;
inline FontCache fontCache;
inline MusicCache musicCache;
inline TileSet tileset;
inline TileSet spriteSheet;
inline ScoreTable scoreTable;
inline bool isMenu = false;

using collision_type = void(CollisionData, CollisionData);
using collision_signal = entt::sigh<collision_type>;
using collision_sink = entt::sink<collision_type>;
