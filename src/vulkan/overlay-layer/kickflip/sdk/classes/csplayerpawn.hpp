#pragma once

#include "cweapon.hpp"
#include "bonearray.hpp"
#include "collisionproperty.hpp"
#include "../../utils.hpp"
class CSPlayerPawn { //todo: move to base
public:
    CSPlayerPawn(uintptr_t h){
        checked = false;
        valid = false;
        handle = h;
        validate();
    }
    bool validate(){
        checked = true;
        if(handle > 0)
            valid = true;
        return valid;
    }
    ~CSPlayerPawn(){}

    static void clearOffsets()
    {
       
    }


public:
    uintptr_t bones(){
        
        if(!m_pGameSceneNode)
            m_pGameSceneNode = kf->off->get(xs("C_BaseEntity"), xs("m_pGameSceneNode"));
        if(!m_modelState)
            m_modelState = kf->off->get(xs("CSkeletonInstance"), xs("m_modelState"));
        if(!(m_modelState > 0) || !(m_pGameSceneNode > 0))
            return 0;
        uintptr_t gamescenenode = *(uintptr_t *)(handle + m_pGameSceneNode);
        if(!gamescenenode)
            return 0;
        uintptr_t bonearray = *(uintptr_t*)(gamescenenode + m_modelState + oBoneArray);
        return bonearray;
       
    }
    Vector3 origin(){
         if(!m_pGameSceneNode)
            m_pGameSceneNode = kf->off->get(xs("C_BaseEntity"), xs("m_pGameSceneNode"));
        if(!m_vecOrigin)
            m_vecOrigin = kf->off->get(xs("CGameSceneNode"), xs("m_vecOrigin"));
        if(!(m_vecOrigin > 0) || !(m_pGameSceneNode > 0))
            return Vector3{};
        uintptr_t gamescenenode = *(uintptr_t *)(handle + m_pGameSceneNode);
        if(!gamescenenode)
            return Vector3{};
        Vector3 ret = *(Vector3*)(gamescenenode + m_vecOrigin);
        return ret;
    }
    Vector3 viewOffset(){
        if(!m_vecViewOffset)
         m_vecViewOffset = kf->off->get(xs("C_BaseModelEntity"), xs("m_vecViewOffset"));
         if(!m_vecViewOffset)
            return Vector3{};
        Vector3 offset = *(Vector3*)(handle + m_vecViewOffset);
        return offset;
    }
    Vector3 eyePosition(){

        return Util::AddVector3(origin(), viewOffset());
    }
    bool defusing(){
        if(!m_bIsDefusing)
            m_bIsDefusing = kf->off->get(xs("C_CSPlayerPawnBase"), xs("m_bIsDefusing"));
        if(!m_bIsDefusing)
            return true; //make this clear 
        return *(uint8_t *)(handle + m_bIsDefusing); //tbh shoulda just done a getHandle that always 0 checked :/ 
        
    }
    bool movedSinceSpawn(){
        if(!m_bHasMovedSinceSpawn)
            m_bHasMovedSinceSpawn = kf->off->get(xs("C_CSPlayerPawnBase"), xs("m_bHasMovedSinceSpawn"));
        if(!m_bHasMovedSinceSpawn)
            return false;
        
        bool moved = *(bool*)(handle + m_bHasMovedSinceSpawn);
        return moved;
    }
    QAngle aimPunchAngle(){
        QAngle ret; ret.x =69.f; ret.y=69.f;
        if(!m_aimPunchAngle)
            m_aimPunchAngle = kf->off->get(xs("C_CSPlayerPawn"), xs("m_aimPunchAngle"));
        if(!m_aimPunchAngle)
            return ret;
        ret = *(QAngle*)(handle + m_aimPunchAngle);
        return ret;
        
    }
    QAngle eyeAngle(){
        QAngle ret; ret.x = 69.f; ret.y= 69.f;
        if(!m_angEyeAngles)
            m_angEyeAngles = kf->off->get(xs("C_CSPlayerPawnBase"), xs("m_angEyeAngles"));
        if(!m_angEyeAngles)
            return ret;
         ret = *(QAngle*)(handle + m_angEyeAngles);
        return ret;
    }
    Vector3 oldOrigin(){
        if(!m_vOldOrigin)
            m_vOldOrigin = kf->off->get(xs("C_BasePlayerPawn"), xs("m_vOldOrigin"));
        //this worked. i believe
        Vector3 ret = *(Vector3*)(handle + m_vOldOrigin);
        return ret;
        
    }
    uintptr_t addy(){ //swing the addy fam
            return handle;
        }
    
    CWeapon clippingWeapon(){
        if(!m_pClippingWeapon )
            m_pClippingWeapon = kf->off->get(xs("C_CSPlayerPawnBase"), xs("m_pClippingWeapon"), false);
        if(!m_pClippingWeapon)
            return CWeapon((uintptr_t)0); 
        uintptr_t h = *(uintptr_t *)(handle + m_pClippingWeapon);
        return h;
        
    }
    float sensitivity(){
        if(!m_flMouseSensitivity)
            m_flMouseSensitivity = kf->off->get(xs("C_BasePlayerPawn"), xs("m_flMouseSensitivity"));
        if(!m_flMouseSensitivity)
            return 0.f;
        
        float sens = 0.f;
        sens = *(float*)(handle + m_flMouseSensitivity);
        return sens;
        
    }
    uintptr_t pCollision(){ //ccollisionproperty
        if(!m_pCollision)
            m_pCollision = kf->off->get(xs("C_BaseEntity"), xs("m_pCollision"));
        if(!m_pCollision)
            return 0;
        uintptr_t h = *(uintptr_t*)(handle + m_pCollision);

        return h;
    }
    uintptr_t hOwnerEntity(){ //chandle
        if(!m_hOwnerEntity)
            m_hOwnerEntity = kf->off->get(xs("C_BaseEntity"), xs("m_hOwnerEntity"));
         if(!m_hOwnerEntity)
            return 0;
        return *(uintptr_t*)(handle + m_hOwnerEntity);
        
    }
    uint32_t flags(){ //chandle
        if(!m_fFlags)
            m_fFlags = kf->off->get(xs("C_BaseEntity"), xs("m_fFlags"));
         if(!m_fFlags)
            return 0;
        uint32_t flg = *(uint32_t*)(handle + m_fFlags); //DOESNT WORK!!!
        return flg;
    }
    int32_t health(){
            if(!m_iHealth){
                m_iHealth = kf->off->get(xs("C_BaseEntity"), xs("m_iHealth"));
            }
            if(!m_iHealth)
                return -1;
           
            int32_t hp = *(int32_t*)(handle + m_iHealth);
            return ( (hp >= 0) && (hp <= 100) ) ? hp : -1;
        }
    uint8_t teamNum(){
        if(!m_iTeamNum)
            m_iTeamNum = kf->off->get(xs("C_BaseEntity"), xs("m_iTeamNum"));
        if(!m_iTeamNum || !validate())
            return 0;
        
        uint8_t team = *(uint8_t*)(handle + m_iTeamNum);
        return team;
    }
    EntitySpottedState_t spotted(){
            if(!m_entitySpottedState){
                m_entitySpottedState = kf->off->get(xs("C_CSPlayerPawnBase"), xs("m_entitySpottedState"));
            }
            if(!m_entitySpottedState)
                return EntitySpottedState_t{};
           
            EntitySpottedState_t spot = *(EntitySpottedState_t*)(handle + m_entitySpottedState);

            


            return spot;
        }
    uintptr_t getWeaponServices(){
        uintptr_t m_pWeaponServices = kf->off->get(xs("C_BasePlayerPawn"), xs("m_pWeaponServices"));
        if(!m_pWeaponServices)
            return 0;
        
        return *(uintptr_t*)(handle + m_pWeaponServices);
    }
protected:
    uintptr_t handle;
    bool valid;
    bool checked;

public:
    //OFFSETS
   // static uintptr_t m_vecOrigin; //WHERE DOES THIS OCME FROM! 
    //C_BaseEntity
    static uintptr_t m_pGameSceneNode;
    static uintptr_t m_vecOrigin;
    static uintptr_t m_pCollision;
    static uintptr_t m_hOwnerEntity;
    static uintptr_t m_fFlags;
    static uintptr_t m_iHealth;
    static uintptr_t m_iTeamNum;
    //C_BaseModelEntity
    static uintptr_t m_vecViewOffset;

    //CSkeletonInstance
    static uintptr_t m_modelState; 
    //C_CSPlayerPawnbase
    static uintptr_t m_bIsDefusing; 
    static uintptr_t m_bHasMovedSinceSpawn;
    static uintptr_t m_angEyeAngles;
    static uintptr_t m_entitySpottedState;
    //C_CSPlayerPawn
    static uintptr_t m_aimPunchAngle; 

    static uintptr_t m_vOldOrigin;//C_BasePlayerPawn
    
    static uintptr_t m_pClippingWeapon;
    static uintptr_t m_flMouseSensitivity; //CBasePlayerPawn

    
};
