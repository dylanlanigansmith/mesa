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

#include "hook/DetourHooking.hpp"
enum{
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_ERROR
};
#define MAX_LOGS cfg->settings.KF.max_logs


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

        DetourHooking::Hook* FrameStageHook;
        bool hasHookedFSN;
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

        void LogBone(std::string msg, float dist){

            #ifdef LOG_BONES
             Log(msg + std::to_string(dist));
            #endif
            return; 
        }
        void DebugState(std::string str){
            debug = str;
        }
        void DebugState(std::string str, int ar){
            debug = str + " " + std::to_string(ar);
        }
        void DebugState(std::string str, uintptr_t hex);
        std::string ToHex(uint32_t t){
              char buf[128];
              sprintf(buf, ": 0x%lx", t);
              return std::string(buf);
        }
        std::string ToHex(int32_t t){
              char buf[128];
              sprintf(buf, ": 0x%lx", t);
              return std::string(buf);
        }
        std::string ToHex(unsigned char t){
              char buf[128];
              sprintf(buf, ": 0x%x", t);
              return std::string(buf);
        }
    private:
        std::string getLogPrefix(uint8_t type);
        void addLog(std::string log, uint8_t type);
        

        void VirtualInsanity(moduleptr_t &cl);
        std::string virtual_err;
        uintptr_t** vtable_ptr;
        uintptr_t* modifiedVTable;
        uintptr_t oldFunctionPtr;
    private:
        AIM* aim;
        mousetrap* v_mouse; 
        ESP esp;
        Vector2 bounds;
        std::vector<std::string> logs;
        std::string debug;
        bool success;
        Memory* mem;
        InputManager* in;
        bool status;
        CGUI gui;
        std::string version;
        bool stateChangeLastFrame;

        uintptr_t LocalPlayerOffset;
        uintptr_t EntListOffset;
        uintptr_t ViewMatrixOffset;
        uintptr_t PlantedC4Offset;

        uintptr_t TraceShapeOffset;
        uintptr_t EngineTracePtrOffset;
        bool foundOffsets;

        void SigScan();


    public:
        uintptr_t EntityList(){
            if(!foundOffsets)
                SigScan();
           

            return EntListOffset;
        }
        uintptr_t PlantedC4Global(){
            if(!foundOffsets)
                SigScan();
           
                
            return PlantedC4Offset;
        }
        uintptr_t ViewMatrix(){
            if(!foundOffsets)
                SigScan();
           
                
            return ViewMatrixOffset; 
        }
        uintptr_t LocalPlayerGlobal(){
            if(!foundOffsets)
                SigScan();
            
                
            return LocalPlayerOffset; 
        }
        uintptr_t TraceShape(){
            if(!foundOffsets)
                SigScan();
           
                
            return TraceShapeOffset; 
        }
        uintptr_t EngineTracePtr(){
            if(!foundOffsets)
                SigScan();
           
                
            return EngineTracePtrOffset; 
        }
        uintptr_t WeaponC4Global(){
            if(!foundOffsets)
                SigScan();
           
                
            return 0x03554610; //48 8D 15 ? ? ? ? 8B 02 83 E8 01  
            //.text:0000000000EB3D50 48 8D 15 F9 A6 69 02  lea     rdx, Weapon_c4 //if 48 8D 15 then res = +3 
        }
        uintptr_t GameRules(){
            if(!foundOffsets)
                SigScan();
           
                
            return 0x3691748;
        }

        
};

extern KickFlip* kf;