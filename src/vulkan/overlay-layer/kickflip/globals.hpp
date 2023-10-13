#pragma once
#include <string>
#include <cstdint>
#include "includes.hpp"
class Constants{
    public:
      //  static constexpr auto NAVSYSTEM_LIB = "libscenefilecache.so";
      //  static constexpr auto CLIENT_LIB = "libclient.so";
       // static constexpr auto SCHEMA_LIB = "libschemasystem.so";
       // static constexpr auto SDL_LIB = "libSDL3.so.0";
};

#define CLIENT_LIBX xc("libclient.so")
#define SCHEMA_LIBX xc("libschemasystem.so")
#define NAVSYSTEM_LIBX xc("libscenefilecache.so")
#define SDL_LIBX xc("libSDL3.so.0")
#define ENGINE_LIBX xc("libengine2.so")

