#pragma once

#include "../includes.hpp"
#include "../globals.hpp"
#include "../frame.hpp"

namespace sdl{
    static void WarpMouseInWindow(void* window, float x, float y){ //https://wiki.libsdl.org/SDL3/SDL_WarpMouseInWindow
        auto& mod = kf->GetMem()->GetModule(SDL_LIBX);
        if(mod->start){
            const CPointer fn = mod->GetProcAddr(xc("SDL_WarpMouseInWindow"));
            if(fn.IsValid())
                return fn.Call<decltype(&WarpMouseInWindow)>(window, x, y);
        }
        
      
    }
}
