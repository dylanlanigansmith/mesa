#pragma once

#include "baseclass.hpp"

class CC4 : public CBase
{
public:
    static uintptr_t global_addr;
    CC4()
    {
        if (global_addr == 0)
        {
            auto &cl = kf->GetMem()->GetModule(CLIENT_LIBX);
            if (cl)
            {
                if (cl->start > 0 && kf->WeaponC4Global() > 0)
                    global_addr = cl->start + kf->WeaponC4Global();
            }
        }
    }
    ~CC4() {}
     Vector3 origin(){
        if(!getLocal())
            return Vector3{};
        uintptr_t ent = getEntity();
        if(!ent)
            return Vector3{};
        Vector3 C4origin = *(Vector3 *)(ent + 0x70);
       // if (C4origin.x == 0.f || C4origin.y == 0.f || C4origin.z == 0.f) //dumb
       //     return Vector3{}; // really dumb
        return C4origin;
    }
    bool exists(){
        if(!ok())
            return false;
        if(!inPlay())
            return false;
        else if(getLocal() > 0)
            return true;
        return false;
    }
    bool ok()
    {
        if (global_addr > 0)
            return true;
        return false;
    }
    bool inPlay(){
        if(!ok())
            return false;
        
        bool yes = *(bool *)(global_addr); 
      
        return yes;

    }
    bool dropped(){
        if(exists()){
            if(uintptr_t ownerHandle = hOwnerEntity(); ownerHandle == 0x600ffffffff){
                return true;
            }
        }
        return false;
    }
    uintptr_t getOwnerAddress(uintptr_t entity_list ){
        if(exists()){
            //if i wrote a good API this would be GetEntityFromHandle...
            if(uintptr_t ownerHandle = hOwnerEntity(); ownerHandle > 0xff && ownerHandle < 0xfffffff){
                 uintptr_t list_entry2 = *(uintptr_t *)(entity_list + 0x8 * ((ownerHandle & 0x7FFF) >> 9) + 16);
                         if (list_entry2){
                                 uintptr_t c4Owner = *(uintptr_t *)(list_entry2 + 120 * (ownerHandle & 0x1FF));
                                 if(c4Owner)
                                 {
                                    return c4Owner;
                                 }
                         }
            }
        }
        return 0;
    }
    uintptr_t hOwnerEntity(){
         uintptr_t m_hOwnerEntity = kf->off->get(xs("C_BaseEntity"), xs("m_hOwnerEntity"));
         if(!m_hOwnerEntity || !checkHandle())
            return 0;
        return *(uintptr_t*)( handle + m_hOwnerEntity); //game crashes here
    }


private:
    bool checkHandle()
    {
        if (!handle)
            getLocal();
        if (!handle)
            return false;
        return true;
    }
    uintptr_t getLocal()
    {
        if (!ok())
            return 0;
        uintptr_t decoy = *(uintptr_t *)(global_addr + 0x8); // niceee
        if (decoy)
        {
            uintptr_t pc4 = *(uintptr_t *)(decoy);
            handle = (pc4 > 0) ? pc4 : 0;
            if (pc4)
                return pc4;
        }
        return 0;
    }
    uintptr_t getEntity()
    {
        if (!handle)
            getLocal();
        if (!handle)
            return 0;

        uintptr_t C4Entity = *(uintptr_t *)(handle + 0x30); // nice hardcode.. //is this gamescene node
        if (!C4Entity)
            return 0;
        return C4Entity;
    }
};

  /*     // bool armed = *(bool*)(C_C4 + 0x2804);
                      //   DebugState("arm ", (int)armed);
                       // CSPlayerPawn srsly = CSPlayerPawn(C_C4);
                        //DebugState(ToHex((uint32_t)srsly.hOwnerEntity()));
              
                } strings "planting" "carried"
                    if ( (int)CPlantedC4 <= 0 )
                    {
                    LABEL_45:
                        result = (unsigned int)(Weapon_c4 - 1);
                        if ( (int)Weapon_c4 - 1 < 0 )
                        return result;
                        Weapon_c4p = *(&Weapon_c4 + 1);
                        result = (int)result;
                        while ( 1 )
                        {
                         C_C4Ent = *(_QWORD *)(Weapon_c4 + 8 * result);
                            if ( C_C4Ent )
                                break;
                            if ( (int)--result < 0 )
                                return result;
                        }
                        ...

                */

                /*
                                //0x3691748 E8 ? ? ? ? 48 8D 05 ? ? ? ? 48 8B 38 48 85 FF 74 30 // E8 AE 25 2A 00                          call    sub_CB6910
                                                                            .text:0000000000A14362 48 8D 05 FF 71 C7 02                    lea     rax, CSGameRules
                   if ( !*((_BYTE *)CSGameRules + 0x9DC) ) //m_bombDropped
                        v7 = "carried"; 
                
                */