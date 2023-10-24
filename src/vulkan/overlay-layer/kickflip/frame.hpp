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

enum Funcs{
    SetMeshGroupMask = 0,
    CreateSharedObject,
    GetInvManager,
    SetDynamicAttributeValueUint,
    GetGameCoordinatorSystem,
    FindSharedObjectCache,
    CreateTypeCache,

};
//CALL_VIRTUAL(base, retType, idx, ...) 
// * For inv change:
  
 // SET_MESH_GROUP_MASK, E8 ? ? ? ? 4C 89 EF E8 ? ? ? ? 41 89 C6  //0x0B526C0
 //__int64 __fastcall Set_Mesh_Group_Mask(__int64, __int64)
typedef void  __attribute__((fastcall)) (*SetMeshGroupMaskFn)(uintptr_t, uintptr_t);
 // SetMeshGroupMaskFn SetMeshGroupMask = (SetMeshGroupMaskFn)(cl->start + 0x0B526C0);

 //CREATE_SHARED_OBJECT_SUBCLASS_ECON_ITEM  55 BF ? ? ? ? 48 89 E5 E8 ? ? ? ? 48 8D 15 ? ? ? ? 48 8D 8A ? ? ? ? 
//0x011D0CE0 CreateSharedObjectSubclassEconItem   __int64 CreateSharedObjectSubclassEconItem() Vtable 
typedef int64_t  (*CreateSharedObjectSubclassEconItemFn)();

//GetInventoryManager E8 ? ? ? ? 8B 4D E0 
//0x00ACDE90 ; uintptr_t *GET_INVENTORY_MANAGER()
typedef uintptr_t* (*GetInventoryManagerFn)();

//SetDynamicAttributeValueUint 
//0x11ED820 ; void __fastcall SetDynamicAttributeItemUint2_0(__int64, __int64, _DWORD *)
//E9 ? ? ? ? 66 90 55 48 89 E5 53 48 89 FB 48 83 EC 08 48 89 37 //48 89 F7 48 89 D6 48 89 CA E9 ? ? ? ? 66 90 55 48 89 E5 53 

typedef void __attribute__((fastcall))  (*SetDynamicAttributeItemUintFn)(int64_t, int64_t, int32_t);


//0x011DD680 GetGameCoordinatorSystem proc  // E8 ? ? ? ? 48 85 C0 74 0F 4C 89 E6 
typedef uintptr_t* (*GGetGameCoordinatorSystemFn)(); 


//__int64 __fastcall FindSharedObjectCache(__int64 a1, __int64 a2, __int64 a3, char a4) E8 ? ? ? ? 49 89 C5 48 85 C0 74 0E
typedef uintptr_t __attribute__((fastcall))  (*FindSharedObjectCacheFn)(uintptr_t a1, int64_t, int64_t, char); //0x2CC0580

//0x02CD2330 ; __int64 __fastcall SortofCreateTypeCache(__int64, unsigned int)
typedef uintptr_t __attribute__((fastcall))  (*CreateTypeCacheFn)(uintptr_t a1, uint32_t); // E8 ? ? ? ? 48 89 C7 48 8B 07 4C 89 E6 

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
        DetourHooking::Hook* CheckVTablesHook;
        DetourHooking::Hook* DLLStatusHook;
        DetourHooking::Hook* UtilCVarHook;
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
        void Log(uint32_t num, std::string msg, uint8_t type = LOG_DEBUG);
        uintptr_t getFunction(Funcs f);
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
    public:  
        void UnVirtualInsanity(moduleptr_t &cl, int mode);
    private:
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
        uintptr_t SetModelFnOffset;
        uintptr_t WeaponC4Offset;
        uintptr_t SetMeshOffset;
        uintptr_t VMTOffset;
        bool foundOffsets;
        int sig_attempts;

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
           
                
            return WeaponC4Offset; 
        }
        uintptr_t GameRules(){
            if(!foundOffsets)
                SigScan();
           
            Log(xs("unimplemented: GameRules"));
            return -1;
        }
        uintptr_t SetModel(){
            if(!foundOffsets)
                SigScan();
           
                
            return SetModelFnOffset;
        }
         uintptr_t VMTCheck(){
            if(!foundOffsets)
                SigScan();
           
            
            return VMTOffset;
        }
           uintptr_t SetMesh(){
            if(!foundOffsets)
                SigScan();
           
                
            return SetMeshOffset;
        }

        
};

extern KickFlip* kf;