#include "utils.hpp"
#include "s2_types.hpp"
#include "mem/memory.hpp"
#include "frame.hpp"
#include "offsets.hpp"
int64_t Util::ConColorMsg(const char* message, uint8_t clr){
    /*
      .plt:0000000000589320 ; __int64 __fastcall ConColorMsg(__int64, const char *)
      .plt:0000000000589320 __Z11ConColorMsgRK5ColorPKcz proc near                                     
      .plt:0000000000589320                 jmp     cs:_Z11ConColorMsgRK5ColorPKcz_ptr ; ConColorMsg(Color const&,char const*,...)
      .plt:0000000000589320 __Z11ConColorMsgRK5ColorPKcz endp
      __int64 __fastcall ConColorMsg(__int64 a1, const char *a2) //0x3678B62
      {
         return _Z11ConColorMsgRK5ColorPKcz(a1, a2);
      }
      Seen Used As:
      double v364;
      LODWORD(v364) = -32704;
      ConColorMsg((__int64)&v364, "Weapon_GetSlot.. etc") 

      or

      ConColorMsg((__int64)&dword_3678B5E, ")->AmmoEmpty() "); //dword @ 0x3678B5E with a color passed by ref (?)


      Attempted to call directly: 
      extern:0000000003868128                 extrn _Z11ConColorMsgRK5ColorPKcz:near
      */
     uintptr_t addr = 0;
     addr = kf->GetMem()->GetModule(CLIENT_LIBX)->start ;
     if(addr == 0){
        return 1;
     }
      Color msg(255,0,255,255);

      typedef int64_t __attribute__((fastcall)) (*ConColorFn)(int64_t, const char *a2); //TODO: ***** USE GETPROCADDR

      ConColorFn  ConColorMsg = (ConColorFn)(addr + ConColorOffset); //Linux 

      double trble = -32704; //OR just using one of the Colors seen used by the game color &(0x3678B62) 0x3678B5E 0x3678B5A 
      return ConColorMsg((int64_t)(&trble), message);//  
}

int64_t Util::DevWarning(const char* message){
   typedef int64_t __attribute__((fastcall)) (*DevWarningFn)(const char *a2); 
/*
__int64 __fastcall DevWarning(const char *)
.plt:0000000000587500 __Z10DevWarningPKcz proc near           ; CODE XREF: sub_6B0CC0+414↓p
.plt:0000000000587500                                         ; sub_6B0CC0+46B↓p ...
.plt:0000000000587500                 jmp     cs:_Z10DevWarningPKcz_ptr ; DevWarning(char const*,...)
.plt:0000000000587500 __Z10DevWarningPKcz endp

*/
   uintptr_t addr = 0;
     addr = kf->GetMem()->GetModule(CLIENT_LIBX)->start ;
     if(addr == 0){
        return 1;
     }
      DevWarningFn  DevWarning = (DevWarningFn)(addr + DevWarningOffset); //Linux 
      return DevWarning(message);//  
}


Vector3 Util::WorldToScreen(Vector2 bounds, Vector3 pos, struct view_matrix_t matrix){
        float _x = matrix.matrix[0][0] * pos.x + matrix.matrix[0][1] * pos.y + matrix.matrix[0][2] * pos.z + matrix.matrix[0][3];
		float _y = matrix.matrix[1][0] * pos.x + matrix.matrix[1][1] * pos.y + matrix.matrix[1][2] * pos.z + matrix.matrix[1][3];

		float w = matrix.matrix[3][0] * pos.x + matrix.matrix[3][1] * pos.y + matrix.matrix[3][2] * pos.z + matrix.matrix[3][3];
     
		float inv_w = 1.f / w;
		_x *= inv_w;
		_y *= inv_w;

		float x = bounds.x * .5f;  //gb.right
		float y = bounds.y * .5f; //gb.bot

		x += 0.5f * _x * bounds.x + 0.5f; //gb.right
		y -= 0.5f * _y * bounds.y + 0.5f; //gb.bot

		return { x, y, w };

}

std::string Util::GetUserSetting(const char* cv){
  /*
  "GetUserSetting: cvar '%s' unknown.\n"
  2E5E60 ; char *__fastcall GetUserSetting(__int64, const char *)
.text:00000000002E5E60 GetUserSetting  proc near               ; CODE XREF: sub_2DA7E0+8C↑j
.text:00000000002E5E60                                         ; sub_336950+A12↓p ...
.text:00000000002E5E60                 push    rbp
.text:00000000002E5E61                 mov     rbp, rsp
.text:00000000002E5E64                 push    r14
.text:00000000002E5E66                 push    r13
.text:00000000002E5E68                 push    r12
.text:00000000002E5E6A                 push    rbx
.text:00000000002E5E6B                 mov     rbx, rdi
.text:00000000002E5E6E                 mov     rdi, [rdi+120h] ; this
.text:00000000002E5E75                 test    rsi, rsi
.text:00000000002E5E78                 jz      short loc_2E5EC7
.text:00000000002E5E7A                 lea     r13, asc_589F45+3 ; ""
.text:00000000002E5E81                 mov     r14, rsi
.text:00000000002E5E84                 test    rdi, rdi
.text:00000000002E5E87                 jz      short loc_2E5EC7
.text:00000000002E5E89                 nop     dword ptr [rax+00000000h]
  
  
  */ 

  typedef char* __attribute__((fastcall)) (*GetUserSettingFn)(const char *a2);
      uintptr_t addr = 0;
     addr = kf->GetMem()->GetModule(ENGINE_LIBX)->start ;
     if(addr == 0){
        return "";
     }
   
   GetUserSettingFn get = (GetUserSettingFn)(addr + 0x2E5E60);
   char* set = get(cv);

   return std::string(set);
}


uintptr_t Util::VisibilityTrace(C_GameTrace* trace, Vector3* startPos, Vector3* endPos, uintptr_t handle){

   uintptr_t addr = 0;
      addr = kf->GetMem()->GetModule(CLIENT_LIBX)->start ;
     if(addr == 0){
        return 1;
     }
      //Set Up Ray
      C_Ray ray;   
      Vector3 zero; zero.x = 0.f; zero.y = 0.f; zero.z = 0.f;
      ray.UnkType = 0; ray.Start = ray.End = ray.Mins = ray.Maxs = zero;
      TraceShapeFn TraceShape = (TraceShapeFn)(addr + kf->TraceShape());
      uintptr_t etp = *(uintptr_t*)(addr + kf->EngineTracePtr());
      uintptr_t* EngineTracePtr = (uintptr_t*)(etp);
      
                                 //Mask for visibility 
      C_TraceFilter filter((uint32_t)0x1C3003, handle, 0, 4); 
      uintptr_t ret = TraceShape(EngineTracePtr, &ray, startPos, endPos, &filter, trace);

      return ret;
}









Vector3 Util::AddVector3(const Vector3 a, const Vector3 b){
		Vector3 ret;
		ret.x = a.x + b.x;
		ret.y = a.y + b.y;
		ret.z = a.z + b.z;
      return ret;
	}
Vector3 Util::SubVector3(const Vector3 a, const Vector3 b){
		Vector3 ret;
		ret.x = a.x - b.x;
		ret.y = a.y - b.y;
		ret.z = a.z - b.z;
      return ret;
	}

inline double deg2rad(double deg){
   return deg * (M_PI / 180);
}
Vector3 Util::fromAngle(const QAngle angle){
   Vector3 ret;
		ret.x = std::cos(deg2rad(angle.x)) * std::cos(deg2rad(angle.y));
		ret.y = std::cos(deg2rad(angle.x)) * std::sin(deg2rad(angle.y));
		ret.z = -std::sin(deg2rad(angle.x));
      return ret;
}

Vector3 Util::MultiplyScalar(const Vector3 a, double s){
   Vector3 ret;
		ret.x = a.x * s;
		ret.y = a.y * s;
		ret.z = a.z * s;
      return ret;
}

