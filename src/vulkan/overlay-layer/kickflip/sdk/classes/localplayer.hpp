#pragma once

#include "playercontroller.hpp"


class LocalPlayer : public PlayerController { //god i should have made a BasePlayerController Class and inherited 
    public:
        LocalPlayer(moduleptr_t &mod){
            handle = refreshHandle();
            
            //assuming we are in game god bless us
            client_start = 0;
            client_start = mod->start;
            
            
        }
        ~LocalPlayer(){

        }
        bool validate(){
            checked = true;

            uintptr_t v_handle = refreshHandle();
            if(v_handle > 0 && client_start > 0){
                handle = v_handle; //minds well
                valid = true;
                return valid;
            } //smarter way to do this
            return false;
        }
        bool isAlive(){
            if(validate()){ //bc might be called as a check, tho look into connected state
                if(!m_bPawnIsAlive)
                    m_bPawnIsAlive = kf->off->get(xs("CCSPlayerController"),xs("m_bPawnIsAlive"));
                if(!m_bPawnIsAlive)
                    return false;
                
                bool alive = *(bool*)(handle + m_bPawnIsAlive);
                return alive;
            }
            return false;
        }
       

    private:
        uintptr_t refreshHandle()
        {
            if(!client_start)
                return 0;
            uintptr_t newhandle = *(uintptr_t*)(client_start + LocalPlayerController);
            if(!newhandle)
                return 0;
            return newhandle;
        }
       
    private:
      

};