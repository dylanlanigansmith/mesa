#pragma once

#include "baseclass.hpp"

class CSGameRules {
    public:
    CSGameRules(){

    }
    ~CSGameRules(){}
    bool  m_bWarmupPeriod(){
        uintptr_t netvar = kf->off->get(xs("C_CSGameRules"), xs("m_bWarmupPeriod"));
        if(netvar)
            return *(bool*)(this + netvar);
        return false;
    }
    bool  m_bTeamIntroPeriod(){
        uintptr_t netvar = kf->off->get(xs("C_CSGameRules"), xs("m_bTeamIntroPeriod"));
        if(netvar)
            return *(bool*)(this + netvar);
        return false;
    }
    bool m_bMapHasRescueZone(){
        uintptr_t netvar = kf->off->get(xs("C_CSGameRules"), xs("m_bMapHasRescueZone"));
        if(netvar)
            return *(bool*)(this + netvar);
        return false;
    }
    
};

 /*
                                //0x3691748 E8 ? ? ? ? 48 8D 05 ? ? ? ? 48 8B 38 48 85 FF 74 30 // E8 AE 25 2A 00                          call    sub_CB6910
                                                                            .text:0000000000A14362 48 8D 05 FF 71 C7 02                    lea     rax, CSGameRules
                   if ( !*((_BYTE *)CSGameRules + 0x9DC) ) //m_bombDropped
                        v7 = "carried"; 
                
                */