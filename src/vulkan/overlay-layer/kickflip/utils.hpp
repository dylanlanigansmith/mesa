#pragma once
#include "includes.hpp"
#include "s2_types.hpp"
enum {
   CONCOLOR_WHITE = 0,
   CONCOLOR_RED,
   CONCOLOR_YELLOW,
   CONCOLOR_GREEN,
};
class Util{
    public:
        static int64_t ConColorMsg(const char* message, uint8_t clr = 0);
        static int64_t DevWarning(const char* message);
        static Vector3 WorldToScreen(Vector2 bounds, Vector3 pos, struct view_matrix_t matrix);
        static std::string GetUserSetting(const char* cv);
        static Vector3 AddVector3(const Vector3 a, const Vector3 b);
        static Vector3 Util::fromAngle(const QAngle angle);
        static Vector3 MultiplyScalar(const Vector3 a, double s);
};

