#include "enginetrace.hpp"

#include "classes/csplayerpawn.hpp"
#include "../frame.hpp"


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
   
    if(!skip1.validate())
        return;
  GetHandleFromEntityFn GetEntityHandle = (GetHandleFromEntityFn)(addr + HANDLEFROM_OFFSET);
  uintptr_t handle = GetEntityHandle(Skip1);
  if(!handle){
    kf->Log("bad handle"); return;
  }
  if(!skip1.hOwnerEntity()){
    kf->Log("bad owner"); return;
  }
  SkipHandles[0] = handle;
  SkipHandles[1] = handle;
  SkipHandles[2] = skip1.hOwnerEntity();
  SkipHandles[3] = skip1.hOwnerEntity();
  uintptr_t h = skip1.pCollision();
  if(!h){
    kf->Log("bad Collision"); return;
  }
   
  CollisionProperty col = CollisionProperty(h);
  Collisions[0] = col.CollisionMask(); //should validate
  Collisions[1] = col.CollisionMask();
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