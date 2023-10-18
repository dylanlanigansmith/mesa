#pragma once

#include "baseclass.hpp"

#define C4_TIME 40.f
class PlantedC4 : public CBase {
    public:
    PlantedC4(){
        if(global_addr == 0){
            auto& cl =  kf->GetMem()->GetModule(CLIENT_LIBX);
           if( cl){
                if(cl->start > 0 && kf->PlantedC4Global() > 0)
                    global_addr = cl->start + kf->PlantedC4Global();
           }
        }
        
    }
    bool isPlanted(){
        if(!ok())
            return false;
        
        bool yes = *(bool *)(global_addr); //*(bool *)(client_start + GlobalPlantedC4)
        //kf->Log((yes) ? "ispLanted" : "no planted");
        return yes;
    } 
    float TimeRemaining(){
         if(!handle)
                getLocal();
        if(!handle)
                return -1.f;
        
        
        float TotalTimer = timer_now();       // m_fBombTimeTotal
        float PlantedTimestamp = time_planted(); // m_fBombTimeStart <- ig i made these up lol
        float timeRemaining = C4_TIME - (TotalTimer - PlantedTimestamp);
        if(timeRemaining < 0.f)
            timeRemaining = 0.f;
        return timeRemaining;
    }

//C_PlantedC4 m_flDefuseLength  m_flDefuseCountDown
    bool defusing(){
        if(!checkHandle())
                return false;
        uintptr_t m_bBeingDefused = kf->off->get(xs("C_PlantedC4"), xs("m_bBeingDefused"));
        if(!( m_bBeingDefused > 0))
            return false; //so we know somethin wrong true but also this could b bad if we use this as a check to access defusing player ent so false 
        
        return *(bool*)(handle + m_bBeingDefused);
        
    }//m_bBombDefused
     bool defused(){
        if(!checkHandle())
                return false;
        uintptr_t m_bBombDefused = kf->off->get(xs("C_PlantedC4"), xs("m_bBombDefused"));
        if(!( m_bBombDefused > 0))
            return false; 
        
        return *(bool*)(handle + m_bBombDefused); //m_bHasExploded
        
    }
    bool exploded(){
        if(!checkHandle())
                return false;
        uintptr_t m_bHasExploded = kf->off->get(xs("C_PlantedC4"), xs("m_bHasExploded"));
        if(!( m_bHasExploded > 0))
            return false; 
        
        return *(bool*)(handle + m_bHasExploded); //m_bHasExploded
        
    }
    float defuselength(){
        if(!checkHandle())
            return 0.f;
        if(!defusing())
            return -1.f;
        
         uintptr_t m_flDefuseLength = kf->off->get(xs("C_PlantedC4"), xs("m_flDefuseLength")); 
         uintptr_t m_flDefuseCountDown = kf->off->get(xs("C_PlantedC4"), xs("m_flDefuseCountDown"));
        if(!( m_flDefuseLength  > 0) || !( m_flDefuseCountDown > 0))
            return -2.f;
        
        float length = *(float *)(handle + m_flDefuseLength); //we might need 2 get entity 
        float count = *(float *)(handle + m_flDefuseCountDown);
      
        float time_remaining = (timer_now() - count); 
       
        return time_remaining; // return countdown; //reclass it up! 

    }
    
    bool defusingwithkit(){
        if(!checkHandle())
            return false;
        if(!defusing())
            return false;
        
         uintptr_t m_flDefuseLength = kf->off->get(xs("C_PlantedC4"), xs("m_flDefuseLength")); 
         
        if(!( m_flDefuseLength  > 0) )
            return false;
        
        float length = *(float *)(handle + m_flDefuseLength); //we might need 2 get entity 
        

        return (9 > length); //5 s or 10s

    }


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
        if(!isPlanted())
            return false;
        else if(getLocal() > 0)
            return true;
        return false;
    }
      bool ok(){
            if(global_addr > 0)
                return true;
            return false;
        }
     static uintptr_t global_addr;
    private:
        float time_planted(){
            return *(float *)(handle + 0x434);
        }
         float timer_now(){
            return *(float *)(handle + 0x430);
        }
        bool checkHandle(){
             if(!handle)
                getLocal();
            if(!handle)
                return false;
            return true;
        }
        uintptr_t getLocal(){
            if(!ok())
                return 0;
            uintptr_t decoy = *(uintptr_t *)(global_addr + 0x8); //niceee
            if(decoy){
                 uintptr_t pc4 = *(uintptr_t *)(decoy);
                 handle = (pc4 > 0 ) ? pc4 : 0;
                 if(pc4)
                    return pc4;
            }
            return 0;
        }
        uintptr_t getEntity(){
            if(!handle)
                getLocal();
            if(!handle)
                return 0;

            uintptr_t C4Entity = *(uintptr_t *)(handle + 0x30);  //nice hardcode.. //is this gamescene node
            if(!C4Entity)
                return 0;
            return C4Entity;
        }
      
        
        
};
/*
if (*(bool *)(client_start + GlobalPlantedC4) && cfg->settings.ESP.BombTimer)
            {

                uintptr_t decoy = *(uintptr_t *)(client_start + GlobalPlantedC4 + 0x8);
                if (decoy)
                {
                    uintptr_t PlantedC4 = *(uintptr_t *)(decoy);

                    if (PlantedC4)
                    {

                        float TotalTimer = *(float *)(PlantedC4 + 0x430);       // m_fBombTimeTotal
                        float PlantedTimestamp = *(float *)(PlantedC4 + 0x434); // m_fBombTimeStart
                        uintptr_t C4Entity = *(uintptr_t *)(PlantedC4 + 0x30);  // m_fBombTimeTotal
                        if (!C4Entity)
                            goto null;
                        Vector3 C4origin = *(Vector3 *)(C4Entity + 0x70);
                        if (C4origin.x == 0.f || C4origin.y == 0.f || C4origin.z == 0.f) // dumb
                            goto null;
                        C4origin.z = C4origin.z + 10.f;
                        // todo make higher
                        Vector3 sC4 = Util::WorldToScreen(bounds, C4origin, matrix);
                        if (sC4.z > 0.01f)
                        { // todo: investigate
                            ImVec4 org = ImVec4(235.f / 255, 139.f / 255.f, 76.f / 255.f, 1);
                            ImVec4 grn = ImVec4(198.f / 255, 250.f / 255.f, 76.f / 255.f, 1);
                            float timeRemaining = 40.f - (TotalTimer - PlantedTimestamp);
                            // ImGui::Text("Bomb: %.1f", timeRemaining);
                            if (timeRemaining < 0.f)
                                timeRemaining = 0.f;
                            char tr[5];
                            sprintf(tr, "%.1f", timeRemaining);
                            ImGui::GetOverlayDrawList()->AddText(ImVec2(sC4.x, sC4.y), ImGui::ColorConvertFloat4ToU32((timeRemaining > 10.f) ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1)), tr);
                        }
                    }
                }
               
            } //end PC4

*/