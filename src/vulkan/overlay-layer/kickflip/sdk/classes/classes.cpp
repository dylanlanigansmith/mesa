#include "playercontroller.hpp"
#include "csplayerpawn.hpp"
#include "cweapon.hpp"
#include "collisionproperty.hpp"

//https://stackoverflow.com/questions/74587411/why-static-global-variables-initialized-to-zero-but-static-member-variable-in-c

uintptr_t PlayerController::client_start = 0;
uintptr_t PlayerController::m_iTeamNum = 0;
uintptr_t PlayerController::m_iHealth = 0;

uintptr_t PlayerController::m_isConnected = 0;
uintptr_t PlayerController::m_hPlayerPawn = 0;
uintptr_t PlayerController::m_bPawnHasDefuser = 0;
uintptr_t PlayerController::m_iPawnHealth = 0;
uintptr_t PlayerController::m_bPawnIsAlive = 0;
uintptr_t PlayerController::m_sSanitizedPlayerName = 0;

uintptr_t CSPlayerPawn::m_pGameSceneNode = 0;
uintptr_t CSPlayerPawn::m_modelState = 0;
uintptr_t CSPlayerPawn::m_pCollision = 0;
uintptr_t CSPlayerPawn::m_hOwnerEntity = 0;
uintptr_t CSPlayerPawn::m_fFlags = 0;
uintptr_t CSPlayerPawn::m_vecOrigin = 0;
uintptr_t CSPlayerPawn::m_vecViewOffset = 0;
uintptr_t CSPlayerPawn::m_iHealth = 0;


uintptr_t CSPlayerPawn::m_bIsDefusing = 0;
uintptr_t CSPlayerPawn::m_aimPunchAngle = 0;
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