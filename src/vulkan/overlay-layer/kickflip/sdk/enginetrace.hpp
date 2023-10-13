#pragma once

#include "../includes.hpp"
#include "../mem/memory.hpp"
#include "../s2_types.hpp"
/**
 * 
 * GetSurfaceData E8 ? ? ? ? 80 78 16 00   __int64 __fastcall GetSurfaceData(__int64 a1) 0xBDBD70
 * 
 * 
 * __int64 __fastcall TraceShape(__int64 *a1, __int64 a2, float *i, __int64 a4, __int64 a5, __int64 a6) 
 * 
 * E8 ? ? ? ? 48 23 9D ? ? ? ?  0x0BDB030 "Physics/TraceShape (Client)"
 * 
 * 
 *  __int64 __fastcall GetHandleFromEntity(__int64) 0xBDC2A0
 * 
 *  E8 ? ? ? ? 41 89 45 28 
 * 
 * 
 * 0x34CDE30 EngineTracePtr 
 * 
 * 0x03673000 EngineTrace
 * 
 * v14 = GetHandleFromEntity(v16);
 * TraceShape((__int64 *)EngineTracePtr, (__int64)&v23, (float *)&v19, (__int64)&v21, (__int64)v27, (__int64)v35);
*/
                                //__int64 __fastcall GetHandleFromEntity(__int64)
typedef uintptr_t __attribute__((fastcall)) (*GetHandleFromEntityFn)(uintptr_t);
#define HANDLEFROM_OFFSET 0xBDB510

                        // __int64 __fastcall GetSurfaceData(__int64 a1)
typedef uintptr_t __attribute__((fastcall)) (*GetSurfaceDataFn)(uintptr_t);
#define SURFACEDATA_OFFSET 0xBDBD70

//__int64 __fastcall TraceShape(__int64 *a1, __int64 a2, float *i, __int64 a4, __int64 a5, __int64 a6)  
//typedef uintptr_t __attribute__((fastcall)) (*TraceShapeFn)(uintptr_t*, uintptr_t, float*, uintptr_t, uintptr_t, uintptr_t);


#define TRACESHAPE_OFFSET 0xBDC2A0 //these are all fucked

#define ENGINETRACE_OFFSET 0x03673000
#define ENGINETRACEPTR_OFFSET 0x34CDE30


struct C_Ray {
    Vector3 Start;
    Vector3 End;
    Vector3 Mins;
    Vector3 Maxs;
    PAD_CLASS(0x4);
    std::uint8_t UnkType;
  };

  struct C_SurfaceData {
    PAD_CLASS(0x8);
    float PenetrationModifier;
    float DamageModifier;
    PAD_CLASS(0x4);
    int Material;
  };

  struct C_TraceHitboxData {
    PAD_CLASS(0x58);
    int Hitgroup;
    PAD_CLASS(0x4);
    int HitboxId;
  };

  class C_GameTrace {
   public:
    C_SurfaceData* GetSurfaceData();

    int GetHitboxId() {
      if (HitboxData) return HitboxData->HitboxId;
      return 0;
    }

    int GetHitgroup() {
      if (HitboxData) return HitboxData->Hitgroup;
      return 0;
    }

    void* Surface;
    uintptr_t* HitEntity; //CSPlayerPawn
    C_TraceHitboxData* HitboxData;
    PAD_CLASS(0x10);
    std::uint32_t Contents;
    PAD_CLASS(0x58);
    Vector3 EndPos;
    PAD_CLASS(0x1C);
    float Fraction;
    PAD_CLASS(0x10);
  };

  class C_TraceFilter {
   public:
    std::uint64_t TraceMask;
    std::uint64_t V1[2];
    std::uint32_t SkipHandles[4];
    std::uint16_t Collisions[2];
    std::uint16_t V2;
    std::uint8_t V3;
    std::uint8_t V4;
    std::uint8_t V5;

    virtual ~C_TraceFilter() {}
    virtual bool function() { return true; }

    C_TraceFilter(std::uint32_t Mask, uintptr_t Skip1,  
                  uintptr_t Skip2, int Layer); //prev CSPlayerpawn* -> uintptr_t 
  };



class EngineTrace{
    public:
        EngineTrace(){

        }
        ~EngineTrace(){

        }


};

//(uintptr_t*, C_Ray*, Vector3*, Vector3*, C_TraceFilter*, C_GameTrace*)
typedef uintptr_t __attribute__((fastcall)) (*TraceShapeFn)(uintptr_t*, C_Ray*, Vector3*, Vector3*, C_TraceFilter*, C_GameTrace*);