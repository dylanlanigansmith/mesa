#pragma once
#include "../includes.hpp"
#include "../s2_types.hpp"
//#define LOG_BONES

class ESP {
    public:
        ESP(){}
        ~ESP(){}
        static void OnFrame();
        
        void DrawBones(uintptr_t ptrpawn, Vector2 bounds, const view_matrix_t& matrix, bool isDude = false);
        void DrawBoxes();

    private:
        float distance(ImVec2 a, ImVec2 b){
            return sqrt( pow(a.x - b.x, 2) + pow(a.y - b.y, 2) );
        }
};