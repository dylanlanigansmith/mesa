#pragma once

#include "../../includes.hpp"
#include "../sdk.hpp"
#include "../../mem/memory.hpp"
#include "../../frame.hpp"
#include "../../offsets.hpp"
#include "csplayerpawn.hpp"





class PlayerController { 
    public:
        PlayerController(){
           // handle = 0;
          //  pawn_handle = 0;
           // entlist = 0;
        }
        PlayerController(uintptr_t h){
            valid = false;
            checked = false;
            //assuming we are in game god bless us
            
            handle = h;
            validate();

            pawn_handle = 0;
            entlist = 0;
        }
        ~PlayerController(){

        }
      
        bool isAlive(){
            if(handle > 0){ //bc might be called as a check, tho look into connected state
                if(!m_bPawnIsAlive)
                    m_bPawnIsAlive = kf->off->get(xs("CCSPlayerController"),xs("m_bPawnIsAlive"));
                if(!m_bPawnIsAlive)
                    return false;
                bool alive = *(bool*)(handle + m_bPawnIsAlive);
                return alive;
            }
            return false;
        }
        int team(){
            if(!m_iTeamNum){
                m_iTeamNum = kf->off->get(xs("CCSPlayerController"),xs("m_iPendingTeamNum")); //beats me but it works
            }
            if(!m_iTeamNum)
                return Teams::NONE;
            return *(int*)(handle + m_iTeamNum); //WTF
        }
        
        uintptr_t pawn(uintptr_t entlist){ // pass pointer to ent list 
        
           
                
                if (!entlist)
                    return 0;
                if(!m_hPlayerPawn)
                    m_hPlayerPawn = kf->off->get(xs("CCSPlayerController"),xs("m_hPlayerPawn"));
                if(!m_hPlayerPawn)
                    return 0;
                uint32_t playerpawn = *(uint32_t *)((handle + m_hPlayerPawn));
                uintptr_t list_entry2 = *(uintptr_t *)(entlist + 0x8 * ((playerpawn & 0x7FFF) >> 9) + 16);
                if (!list_entry2)
                    return 0;
                uintptr_t CSPlayerPawn = *(uintptr_t *)(list_entry2 + 120 * (playerpawn & 0x1FF));
                if(CSPlayerPawn){
                    return CSPlayerPawn;
                }
                return 0;
           
        }
        std::string name(){
            char buf[128] = {"InvalidName"};
            if(!m_sSanitizedPlayerName )
                m_sSanitizedPlayerName = kf->off->get(xs("CCSPlayerController"), xs("m_sSanitizedPlayerName"), false);
            if(!m_sSanitizedPlayerName )
                return std::string(buf);
            uintptr_t name_addr = *(uintptr_t*)(handle + m_sSanitizedPlayerName);
            if (!name_addr) //< 0 ??s
                 return std::string(buf);
            memcpy(buf, (char*)(name_addr), sizeof(char) * 128);
            return std::string(buf);
        }





        static uintptr_t GetPlayerFromIndex(uintptr_t entlist, int i) //should pass a ref ORRR just use as constructor
        {
            //static PlayerController nullpc = PlayerController{};
            uintptr_t list_entry = *(uintptr_t *)(entlist + (8 * (i & 0x7FFF) >> 9) + 16);
            if (!list_entry)
                return 0;
            uintptr_t player = *(uintptr_t *)(list_entry + 120 * (i & 0x1FF));
            if (!player)
                return 0;

            return player;
        }

    public:
        uintptr_t addy(){ //swing the addy fam
            return handle;
        }
        bool validate(){
            checked = true;
            if(handle > 0)
                valid = true;
            return valid;
        }
    private:
          
    protected:
        bool checked; //has validated..
        bool valid;
        uintptr_t handle;
        
        uintptr_t pawn_handle;
        uintptr_t entlist;
    public:
        static uintptr_t client_start;
        //C_BaseEntity
        static uintptr_t m_iTeamNum; 
        static uintptr_t m_iHealth; //m_iPawnHealth ?? m_bPawnIsAlive ?? 
        //CBasePlayerController
        static uintptr_t m_iPawnHealth;
        static uintptr_t m_bPawnIsAlive; 
        static uintptr_t m_bPawnHasDefuser;
        static uintptr_t m_isConnected; //why use such big data type :(
        static uintptr_t m_hPlayerPawn;
        //CCSPlayerController
        static uintptr_t m_sSanitizedPlayerName ;

};

