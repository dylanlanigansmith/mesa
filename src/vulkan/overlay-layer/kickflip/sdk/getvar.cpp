#include "getvar.hpp"
#include "../globals.hpp"
 void VarMgr::init(bool dump = false){
      //  get("CCSPlayerController", "m_iPawnHealth", dump);
      //  get("CCSPlayerController", "m_iCompTeammateColor", dump);
      //  get("CCSPlayerController", "m_iPawnHealth", dump);
       

     //   get("C_CSPlayerPawnBase", "m_entitySpottedState",dump);

     //   get("C_BasePlayerPawn", "m_pWeaponServices",dump);
     //   get("CPlayer_WeaponServices", "m_hActiveWeapon",dump);
       
      //  get("C_EconItemView", "m_iItemIDHigh",dump); //NO! 
      //  get("C_EconEntity", "m_nFallbackPaintKit",dump);

      //  get("C_PlantedC4", "m_bBombDefused", dump);
       // get("C_PlantedC4", "m_flTimerLength", dump);
      //  get("C_PlantedC4", "m_flDefuseCountDown", dump);

        
    }
      uintptr_t VarMgr::get(std::string clss, std::string field, bool dump = false){
        uintptr_t offset = 0;
        std::string key = clss + field;
        if(auto search = cache.find(key); search == cache.end()){
           // const char* lol = std::string("looking for " + field).c_str();
           
          //  const char* c = clss.c_str();  const char* f = field.c_str();
            auto var = CSchemaManager::GetSchemaOffset(CLIENT_LIBX, clss.c_str(), field.c_str());
            calls++;
            if(!var.has_value()){
                if(dump)
                    Util::ConColorMsg(std::string("No " + clss + "\n").c_str());
                return 0;
            }
                
            offset = var.value();
            if(offset){
               
                
                cache.insert({key, offset});
                if(dump){
                    std::string msg = std::string(clss + "->" + field + " = " + std::to_string(offset) + " \n");
                    
                    Util::ConColorMsg(msg.c_str());
                }
                    
                return offset;
            }
               
            return 0;
        }
        else{
            return cache.at(key);
        }

        return 0;
    }