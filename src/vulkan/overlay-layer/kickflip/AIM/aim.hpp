#pragma once

#include "../includes.hpp"
#include "../mem/memory.hpp"
#include "../s2_types.hpp"
#define SENS 2.42

enum aims{
    OFF = 0,
    HAS_TARGET,
    AT_TARGET,
    IDLE,
    SHUTDOWN,
    LOCKING
};

class AIM{
    public:
        AIM();
        ~AIM();
        void setBounds(Vector2 bds) { bounds = bds; }
        ImVec2 getBounds() { return ImVec2(bounds.x, bounds.y); };
        uint8_t getState() { return state;} //returns AIM STATE MACHINE STATE
        void reset(); //resets state machine and target list
        void tick(moduleptr_t& cl); 
        bool validTarget(uintptr_t lp, uintptr_t enemypawn); 
        uintptr_t ShouldHighlight();
        float userSens(){ return sens;}
    private:
        int getPt(int n1, int n2, float perc)
        {
            return n1 + ((n2 - n1) * perc);
        }

        int getRnd(int min, int max)
        {
            return (rand() % (max - min)) + min;
        }
     /*  void MoveGaussian(int targetX, int targetY) {
            Vector2 pInicial = {0, 0};
            Vector2 pLastMove = {0, 0};
            Vector2 pMedio1;
            Vector2 pMedio2;
            Vector2 pFinal;

            pFinal.x = 0; //targetX;
            pFinal.y = targetY;

            if ((pFinal.x >= pInicial.x && pFinal.y >= pInicial.y) || (pFinal.x <= pInicial.x && pFinal.y <= pInicial.y))
            {
                pMedio1.x = pInicial.x + ((pFinal.x - pInicial.x) / getRnd(4, 8));
                pMedio1.y = pInicial.y + ((pFinal.x - pInicial.x) / getRnd(4, 8));

                pMedio2.x = pInicial.x + ((pFinal.x - pInicial.x) / getRnd(3, 6));
                pMedio2.y = pInicial.y + ((pFinal.x - pInicial.x) / getRnd(3, 6));
            }
            else if ((pFinal.x >= pInicial.x && pFinal.y <= pInicial.y) || (pFinal.x <= pInicial.x && pFinal.y >= pInicial.y))
            {
                pMedio1.x = pInicial.x + ((pFinal.x - pInicial.x) / getRnd(4, 8));
                pMedio1.y = pInicial.y - ((pFinal.x - pInicial.x) / getRnd(4, 8));

                pMedio2.x = pInicial.x + ((pFinal.x - pInicial.x) / getRnd(3, 5));
                pMedio2.y = pInicial.y - ((pFinal.x - pInicial.x) / getRnd(3, 5));
            }
            Vector2 total = {0, 0};

            for (float i = 0; i < 1.01; i += 0.01)
            {
                int xa = getPt(pInicial.x, pMedio1.x, i);
                int ya = getPt(pInicial.y, pMedio1.y, i);
                int xb = getPt(pMedio1.x, pMedio2.x, i);
                int yb = getPt(pMedio1.y, pMedio2.y, i);
                int xc = getPt(pMedio2.x, pFinal.x, i);
                int yc = getPt(pMedio2.y, pFinal.y, i);

                int xm = getPt(xa, xb, i);
                int ym = getPt(ya, yb, i);
                int xn = getPt(xb, xc, i);
                int yn = getPt(yb, yc, i);

                int x = getPt(xm, xn, i);
                int y = getPt(ym, yn, i);

                Vector2 move = {x - pLastMove.x, y - pLastMove.y}; // Relative movement

               // YourMethodHere(move.x, move.y);

                pLastMove.x = x;
                pLastMove.y = y;
            }
        }*/ 
        void run();
        float sens;
        std::thread t_main;
        uint8_t state;
        Vector2 bounds;

        uintptr_t highlight;
        bool shouldHighlight;

        bool lastLockSuccess;
};