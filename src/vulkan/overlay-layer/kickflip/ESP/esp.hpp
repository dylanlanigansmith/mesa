#pragma once
#include "../includes.hpp"
#include "../s2_types.hpp"


class ESP {
    public:
        ESP(){}
        ~ESP(){}
        static void OnFrame();
        void DrawBones(uintptr_t CSPlayerPawn, uintptr_t lp, Vector2 bounds, view_matrix_t matrix, int team, bool is_defusing);
        void DrawBoxes();
};