#include "kickflip/frame.hpp"



#define ENT_OFFSET 0x351C468
#define HEALTH_OFFSET 0x948
#define PAWN_OFFSET 0x93c // wrong from here on
#define TEAM_OFFSET 0x880 // m_iTeamNum 0x537

#define dwSanitizedName 0x790 // 0x7f666efdb38c +0x4
#define vecOrigin 0x11b4
#define VIEW_MATRIX 0x371A6C0 //12

#define LocalPlayerController 0x366C618 //12
#define isDefusing 0x1330 
#define GameSceneNode 0x488//pawn+0488
#define CModelState 0x160//gamescenenode + 160
#define oBoneArray 0x80//gamescenenode + 1e0 (modelstate + 0x80)

#define GlobalPlantedC4 0x37267C0 //48 89 87 ? ? ? ? 48 8D 3D ? ? ? ? E8 ? ? ? ? 4C 89 E7 4C 8B 65 F8 

#define ConColorOffset  0x589310
#define DevWarningOffset 0x587500

/**
 * 
 * 
 * 
 * class CViewSetup
{
public:
	MEM_PAD(0x490);
	float flOrthoLeft; // 0x0494
	float flOrthoTop; // 0x0498
	float flOrthoRight; // 0x049C
	float flOrthoBottom; // 0x04A0
	MEM_PAD(0x38);
	float flFov; // 0x04D8
	float flFovViewmodel; // 0x04DC
	Vector_t vecOrigin; // 0x04E0
	MEM_PAD(0xC); // 0x04EC
	QAngle_t angView; // 0x04F8
	MEM_PAD(0x14); // 0x0504
	float flAspectRatio; // 0x0518
};

void **__fastcall ClientModeShared::OverrideView(int *, __int64) //Find with V_atanf
0xD569E0
 * 
 * 
 * https://github.com/ValveSoftware/source-sdk-2013/blob/0d8dceea4310fde5706b3ce1c70609d72a38efdf/mp/src/game/client/clientmode_shared.cpp#L423
 * 
 * 
 * 
 * 
 * 
 * using ConColorFn as (void* uint64_t __fastcall )
 * ConColorMsg(Color const&,char const*,...)  color &(0x3678B62)
 * 
 * 
 * 
 * 
*/