#pragma once
#include "includes.hpp"
#include "input/input.hpp"
#include "mem/memory.hpp"
#include "ESP/esp.hpp"
#include <vector>
#include "GUI/gui.hpp"
#include "sdk/sdk.hpp"
#include "mouse/mouse.hpp"
#include "AIM/aim.hpp"
enum{
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_ERROR
};
#define MAX_LOGS 300
class KickFlip {
    friend CGUI;
    public:
        KickFlip();
        ~KickFlip();
        bool Initialize();
        InputManager* GetInput();
        Memory* GetMem();
        std::string GetOffsets(std::string origin, std::string clss, std::string var);
        void NewFrame(ImGuiContext* imgui_context, float fps, ImVec2 win_size, ImVec2 win_dim);
        
        mousetrap* GetVMouse() { return (v_mouse->isReady() ? v_mouse : nullptr); }
    public:
        bool ok;
        bool menuOpen;
        VarMgr* off;
        AIM* getAim(){ return aim;}
    public:
        void Log(std::string msg, uint8_t type = LOG_DEBUG);
        void Log(uintptr_t hex, std::string msg,  uint8_t type = LOG_DEBUG);
        void Log(std::string msg, QAngle ang, uint8_t type = LOG_DEBUG);
        void Log(std::string msg, Vector3 ang, uint8_t type = LOG_DEBUG);
        void Log(int num, std::string msg, uint8_t type = LOG_DEBUG);
    private:
        std::string getLogPrefix(uint8_t type);
        void addLog(std::string log, uint8_t type);
    private:
        AIM* aim;
        mousetrap* v_mouse; 
        ESP esp;
        Vector2 bounds;
        std::vector<std::string> logs;
        bool success;
        Memory* mem;
        InputManager* in;
        bool status;
        CGUI gui;
        std::string version;
        
};

extern KickFlip* kf;