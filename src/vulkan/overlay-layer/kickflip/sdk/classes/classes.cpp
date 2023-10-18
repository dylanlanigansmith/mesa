#include "playercontroller.hpp"
#include "csplayerpawn.hpp"
#include "cweapon.hpp"
#include "collisionproperty.hpp"
#include "plantedC4.hpp"
#include "cc4.hpp"
//https://stackoverflow.com/questions/74587411/why-static-global-variables-initialized-to-zero-but-static-member-variable-in-c




/**
 * It would be really nice to rewrite this system..
 * so that 1. classes follow game inheritance
 * 2. classes use game memory aka: *(int*)(this + m_iTeamNum)
 * 3. I under stand the SCHEMA define now for adding netvars...
 * 4. would just using varmgr be okay (multithread access...)
 * 5. does it matter if I add shit to my class but then set it to a pointer in the game (like statics basically)
 * 6. will rewriting this be enough of a benefit to do it now..
*/


uintptr_t PlayerController::client_start = 0;
uintptr_t PlayerController::m_iTeamNum = 0;
uintptr_t PlayerController::m_iHealth = 0;

uintptr_t PlayerController::m_isConnected = 0;
uintptr_t PlayerController::m_hPlayerPawn = 0;
uintptr_t PlayerController::m_bPawnHasDefuser = 0;
uintptr_t PlayerController::m_iPawnHealth = 0;
uintptr_t PlayerController::m_bPawnIsAlive = 0;
uintptr_t PlayerController::m_sSanitizedPlayerName = 0;
uintptr_t PlayerController::m_bIsLocalPlayerController = 0;


uintptr_t CSPlayerPawn::m_pGameSceneNode = 0;
uintptr_t CSPlayerPawn::m_modelState = 0;
uintptr_t CSPlayerPawn::m_pCollision = 0;
uintptr_t CSPlayerPawn::m_hOwnerEntity = 0;
uintptr_t CSPlayerPawn::m_fFlags = 0;
uintptr_t CSPlayerPawn::m_vecOrigin = 0;
uintptr_t CSPlayerPawn::m_vecViewOffset = 0;
uintptr_t CSPlayerPawn::m_iHealth = 0;
uintptr_t CSPlayerPawn::m_iTeamNum = 0;

uintptr_t CSPlayerPawn::m_bIsDefusing = 0;
uintptr_t CSPlayerPawn::m_aimPunchAngle = 0;
uintptr_t CSPlayerPawn::m_entitySpottedState =0;

uintptr_t CSPlayerPawn::m_vOldOrigin = 0;
uintptr_t CSPlayerPawn::m_pClippingWeapon = 0;
uintptr_t CSPlayerPawn::m_flMouseSensitivity = 0;
uintptr_t CSPlayerPawn::m_bHasMovedSinceSpawn = 0;
uintptr_t CSPlayerPawn::m_angEyeAngles = 0;

uintptr_t CWeapon::m_iItemDefinitionIndex = 0; 
uintptr_t CWeapon::m_AttributeManager = 0; 

uintptr_t CollisionProperty::m_vecMins = 0;
uintptr_t CollisionProperty::m_vecMaxs = 0;
uintptr_t CollisionProperty::m_usSolidFlags = 0;


uintptr_t PlantedC4::global_addr = 0;
uintptr_t CC4::global_addr = 0;