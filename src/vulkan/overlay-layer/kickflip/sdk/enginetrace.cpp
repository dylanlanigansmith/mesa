#include "enginetrace.hpp"

#include "classes/csplayerpawn.hpp"
#include "../frame.hpp"

uint64_t GetHandleFromEntityRebuilt(uintptr_t a1) //aka CCSPlayerPawn* a1 
{
  uintptr_t v1; // rax
  unsigned int v2; // r8d
  int v3; // edx
  int v4; // eax

  if ( !a1 )
    return 0xFFFFFFFF;
  v1 = *(uint64_t *)(a1 + 16);
  if ( !v1 )
    return 0xFFFFFFFF;
  v2 = *(uint32_t *)(v1 + 16);
  v3 = 0x7FFF;
  v4 = ((v2 >> 15) - (*(uint32_t*)(v1 + 48) & 1)) << 15;
  if ( v2 != -1 )
    v3 = v2 & 0x7FFF;
  return v3 | (unsigned int)v4;
}


C_TraceFilter::C_TraceFilter(std::uint32_t Mask,
                                       uintptr_t Skip1,
                                       uintptr_t Skip2, int Layer) {
  TraceMask = Mask;
  V1[0] = V1[1] = 0;
  V2 = 7;
  V3 = Layer;
  V4 = 0x49;
  V5 = 0;
    uintptr_t addr = 0;
    addr = kf->GetMem()->GetModule(CLIENT_LIBX)->start ;
     if(addr == 0){
        return;
     }

    CSPlayerPawn skip1(Skip1);
   
    if(!skip1.validate()){
kf->Log("bad pawn"); return;
    }
        
 // GetHandleFromEntityFn GetEntityHandle = (GetHandleFromEntityFn)(addr + HANDLEFROM_OFFSET);
  uintptr_t handle = GetHandleFromEntityRebuilt(Skip1);
  if(!handle){
    kf->Log("bad handle"); return;
  }
  if(!skip1.hOwnerEntity()){
    kf->Log("bad owner"); return;
  }
  SkipHandles[0] = handle;
  SkipHandles[1] = -1; //hOwnerEntity - See windows reverse for deets
  SkipHandles[2] = -1;
  SkipHandles[3] = -1;
  uintptr_t h = skip1.pCollision();
  if(!h){
    kf->Log("bad Collision"); return;
  }
   
  CollisionProperty col = CollisionProperty(h);
uint16_t mask = col.CollisionMask();
if(!mask){
kf->Log("mask 0"); 
}
  Collisions[0] = mask; //should validate
  Collisions[1] = 0;
}

C_SurfaceData* C_GameTrace::GetSurfaceData() {
  
    uintptr_t addr = 0;
    addr = kf->GetMem()->GetModule(CLIENT_LIBX)->start ;
     if(addr == 0){
        return nullptr;
     }
    GetSurfaceDataFn GetSurfData = (GetSurfaceDataFn)(addr + SURFACEDATA_OFFSET);

  return reinterpret_cast<C_SurfaceData*>(GetSurfData((uintptr_t)Surface));
}

