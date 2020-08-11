#include "common.h"

#include <SDL.h>
#include <windows.h>
#include <SDL_image.h>

namespace GUI
{
    ListImages loadImageDirectory(SDL_Renderer* renderer, const std::string& path)
    {
        ListImages result;
        WIN32_FIND_DATAA ffd;
        std::string searchPath = path + "/*.*";
        HANDLE handle = FindFirstFileA(searchPath.c_str(), &ffd);
        if (handle == INVALID_HANDLE_VALUE)
            throw std::runtime_error("Could not open image directory!");
        do {
            const char* fname = ffd.cFileName;
            if (strstr(fname, "png") == nullptr)
                continue;
            std::string fullName = path + "/" + std::string(fname);
            SDL_Texture* tex = IMG_LoadTexture(renderer, fullName.c_str());
            if (tex == 0)
                throw std::runtime_error("Could not open image data!");
            ImageInfo iminfo;
            iminfo.tex = tex;
            iminfo.path = fullName;
            SDL_QueryTexture(tex, nullptr, nullptr, &iminfo.w, &iminfo.h);

            result.push_back(iminfo);
        } while (FindNextFileA(handle, &ffd) != 0);
        FindClose(handle);
        return result;
    }
}
