#include "frame.hpp"
#define VERBOSE_LOGGING
#include "globals.hpp"
#include "schema/schemamgr.hpp"
#include "sdk/SDL.hpp"
#include "cfg/config.hpp"

KickFlip* kf;

KickFlip::KickFlip(){
    menuOpen = false;
    ok = false;
    bounds.x = 0; bounds.y = 0;
    status = false;
    version = xs(KFVERSION);
    //gotta be a better way
  
   foundOffsets = false;
   LocalPlayerOffset = 0;
   EntListOffset = 0;
   TraceShapeOffset = 0;
   EngineTracePtrOffset = 0;
   ViewMatrixOffset = 0;
   PlantedC4Offset = 0;
    hasHookedFSN = false;
    sig_attempts = 0;
}
KickFlip::~KickFlip(){
    in->Shutdown();
    ok = false;
   // delete FrameStageHook;
   CheckVTablesHook->disable();
    delete CheckVTablesHook;
    delete mem;
    delete in;
    delete off;
}
bool KickFlip::Initialize(){
    in = nullptr;
    mem = new Memory();
    mem->Initialize();
    off = new VarMgr();
    esp = ESP();
    aim = new AIM();
    v_mouse = new mousetrap();
    Log(v_mouse->err);
    ok = true;
    stateChangeLastFrame = false;
    return true;
}

InputManager* KickFlip::GetInput(){
    if(in != nullptr){
        return in;
    }
    Log("Access attempt to GetInput when nullptr", (uint8_t)LOG_ERROR);
    static InputManager* null{};
    return null;
}

Memory* KickFlip::GetMem(){
    if(mem != nullptr){
        return mem;
    }
    Log("Access attempt to GetMem when nullptr", (uint8_t)LOG_ERROR);
    static Memory* null{};
    return null;
}
std::string KickFlip::getLogPrefix(uint8_t type){
    std::string prefix = "DEBUG: ";
    switch(type){
        case LOG_DEBUG:
            break;
        case LOG_ERROR:
            prefix = "ERROR: "; break;
        case LOG_INFO:
            prefix = "INFO: ";
           
            break;
        default:
            prefix = "UNKNOWN: "; break;
    }
    return prefix;
}
void KickFlip::addLog(std::string log, uint8_t type){
    logs.push_back(getLogPrefix(type) + log); 
    if(logs.size() > MAX_LOGS)
        logs.erase(logs.begin());
}
void KickFlip::DebugState(std::string str, uintptr_t hex){
     char buf[128];
    sprintf(buf, ": 0x%lx", hex);
    debug =  str + buf;

}
void KickFlip::Log(std::string msg, uint8_t type){
    
    
    #ifndef VERBOSE_LOGGING
                return;
    #endif
    addLog(msg, type);
}
void KickFlip::Log(uintptr_t hex, std::string msg,  uint8_t type = LOG_DEBUG){
    #ifndef VERBOSE_LOGGING
                return;
    #endif
    char buf[128];
    sprintf(buf, ": 0x%lx", hex);
    std::string fmt =  msg + buf;

    addLog(fmt, type);
}
void KickFlip::Log(int log, std::string msg,  uint8_t type = LOG_DEBUG){
    #ifndef VERBOSE_LOGGING
                return;
    #endif
    char buf[128];
    sprintf(buf, ": %i", log);
    std::string fmt =  msg + buf;

    addLog(fmt, type);
}
void KickFlip::Log(uint32_t log, std::string msg,  uint8_t type = LOG_DEBUG){
    #ifndef VERBOSE_LOGGING
                return;
    #endif
    char buf[128];
    sprintf(buf, ": %i", log);
    std::string fmt =  msg + buf;

    addLog(fmt, type);
}

void KickFlip::Log(std::string msg, QAngle ang, uint8_t type = LOG_DEBUG){
    #ifndef VERBOSE_LOGGING
                return;
    #endif
    char buf[64];
    sprintf(buf, " (%f, %f, %f)", ang.x, ang.y, ang.z);
    std::string fmt =  msg + buf;
    addLog(fmt, type);
}
void KickFlip::Log(std::string msg, Vector3 ang, uint8_t type = LOG_DEBUG){
    #ifndef VERBOSE_LOGGING
                return;
    #endif
    char buf[64];
    sprintf(buf, " (%f, %f, %f)", ang.x, ang.y, ang.z);
    std::string fmt =  msg + buf;
    addLog(fmt, type);
}
std::string KickFlip::GetOffsets(std::string origin, std::string clss, std::string var){
    std::string offset = origin + "::" + clss + "->" + var + " = ";

    //SCHEMA(uint8_t, m_iTeamNum, "C_BaseEntity", "m_iTeamNum");
    int32_t off = 0;
    auto get = []{
        //return CSchemaManager::GetSchemaOffset("libclient.so","C_BaseEntity","m_iTeamNum").value();
    };
   // off = get();
    sdl::WarpMouseInWindow(nullptr, 1280, 720);

    return offset + std::to_string(off);


}

uintptr_t KickFlip::getFunction(Funcs f)
{
     auto &cl = kf->GetMem()->GetModule(CLIENT_LIBX);
     if(!cl)
        return 0;
    uintptr_t client_start = cl->start;
    switch(f)
    {
        case SetMeshGroupMask:
            return client_start + 0x0B526C0;
        case CreateSharedObject:
            return client_start + 0x011D0CE0;
        case GetInvManager:
          return client_start + 0x00ACDE90;
        case SetDynamicAttributeValueUint:
            return client_start + 0x11ED820;
        case GetGameCoordinatorSystem:
            return client_start + 0x011DD680;
        case FindSharedObjectCache:
            return client_start + 0x2CC0580;
        case CreateTypeCache:
            return client_start + 0x02CD2330; //problematic
        default:
            Log("getFunc error");
            return 0;

    }
    return 0;
}