#pragma once
#include "baseclass.hpp"

enum WeaponType{
    RIFLESMG = 0,
    SHOTGUN = 1,
    SCOPED,
    PISTOL,
    OTHER
};

class CWeapon : public CBase {
    public:
        using CBase::CBase; //maybe just call this and 0 out local vars
  
    uint16_t itemDefinitionIndex(){
        if(!m_iItemDefinitionIndex || !m_AttributeManager){
            m_iItemDefinitionIndex = kf->off->get(xs("C_EconItemView"), xs("m_iItemDefinitionIndex"), false);
            m_AttributeManager = kf->off->get(xs("C_EconEntity"), xs("m_AttributeManager"), false);
        }
        m_Item = 0x50; //surely this is fine
        if(!valid || !m_iItemDefinitionIndex || !m_AttributeManager)
            return 0;
        uint16_t nitemIndex = *(uint16_t *)(handle + m_AttributeManager + m_Item + m_iItemDefinitionIndex);
        if(nitemIndex > 600 || !nitemIndex)
            return 0;

        
        return nitemIndex;
    }

    void setItemDefinitionIndex(uint16_t idx){
        if(!m_iItemDefinitionIndex || !m_AttributeManager){
            m_iItemDefinitionIndex = kf->off->get(xs("C_EconItemView"), xs("m_iItemDefinitionIndex"), false);
            m_AttributeManager = kf->off->get(xs("C_EconEntity"), xs("m_AttributeManager"), false);
        }
        m_Item = 0x50; //surely this is fine
        if( !m_iItemDefinitionIndex || !m_AttributeManager)
            return;
        
        *(uint16_t *)(handle + m_AttributeManager + m_Item + m_iItemDefinitionIndex) = idx; 
       
    }
     void setSteamID(uint32_t id){
        if( !m_AttributeManager){
           
            m_AttributeManager = kf->off->get(xs("C_EconEntity"), xs("m_AttributeManager"), false);
        }
        uintptr_t  m_iAccountID = kf->off->get(xs("C_EconItemView"), xs("m_iAccountID"), false);
        m_Item = 0x50; //surely this is fine
        if(!valid  || !m_AttributeManager)
            return;
        *(uint32_t *)(handle + m_AttributeManager + m_Item + m_iAccountID) = id; //m_iAccountID
       
    }
    void setCustomName(const std::string& s){
      
        m_Item = 0x50; //surely this is fine
         uintptr_t m_szCustomName = kf->off->get(xs("C_EconItemView"), xs("m_szCustomName"));
         uintptr_t m_szCustomNameOverride = kf->off->get(xs("C_EconItemView"), xs("m_szCustomNameOverride")); //char[161]
        std::string name = xs("kickflipped");
        if(s.length() < 160)
            name = s;
        
        if(!valid || !m_AttributeManager || !m_szCustomNameOverride)
            return;
        memcpy( (char*)(handle + m_AttributeManager + m_Item + m_szCustomNameOverride), name.c_str(), sizeof(char) * 161);
        
       
    }
    void setItemIDHigh(){
         if( !m_AttributeManager){
          
            m_AttributeManager = kf->off->get(xs("C_EconEntity"), xs("m_AttributeManager"), false);
        }
        m_Item = 0x50; //surely this is fine
      
      
        uintptr_t m_iItemIDHigh= kf->off->get(xs("C_EconItemView"), xs("m_iItemIDHigh"), true);
        uintptr_t m_iItemIDLow = kf->off->get(xs("C_EconItemView"), xs("m_iItemIDLow"), true);
        uintptr_t m_iItemID = kf->off->get(xs("C_EconItemView"), xs("m_iItemID"));
       

         if(!valid || !m_AttributeManager || !m_iItemIDLow || !m_iItemIDHigh)
            return;
        *(int32_t *)(handle + m_AttributeManager + m_Item + m_iItemIDHigh) = -1;
        *(int32_t *)(handle + m_AttributeManager + m_Item + m_iItemIDLow) = -1;
       /*  *(uint32_t *)(handle + m_AttributeManager + m_iItemIDLow) = -1;
          *(uint32_t *)(handle + m_iItemIDLow) = -1;
        *(uint32_t *)(handle + m_AttributeManager + m_Item + m_iItemIDHigh) = -1;
         *(uint32_t *)(handle + m_AttributeManager + m_iItemIDHigh) = -1;
          *(uint32_t *)(handle + m_iItemIDHigh) = -1; */ 
       //  kf->Log((handle + m_AttributeManager), "m_item");
       // *(uint32_t *)(handle + m_AttributeManager + m_Item + m_iItemID) = -1;
       
    }
    int32_t getItemIDHigh(){
         if(!m_iItemDefinitionIndex || !m_AttributeManager){
            m_iItemDefinitionIndex = kf->off->get(xs("C_EconItemView"), xs("m_iItemDefinitionIndex"), false);
            m_AttributeManager = kf->off->get(xs("C_EconEntity"), xs("m_AttributeManager"), false);
        }
        m_Item = 0x50; //surely this is fine
        if(!valid || !m_iItemDefinitionIndex || !m_AttributeManager)
            return;
      
        uintptr_t m_iItemIDHigh= kf->off->get(xs("C_EconItemView"), xs("m_iItemIDHigh"));
        uintptr_t m_iItemIDLow = kf->off->get(xs("C_EconItemView"), xs("m_iItemIDLow"));
     
      
        return *(int32_t *)(handle + m_AttributeManager + m_Item + m_iItemIDHigh) ;
        
       
    }
    void setFallBackPaintKit(int32_t pk, int32_t seed, float wear){
        uintptr_t m_nFallbackPaintKit = kf->off->get(xs("C_EconEntity"), xs("m_nFallbackPaintKit"), true);
        uintptr_t  m_nFallbackSeed = kf->off->get(xs("C_EconEntity"), xs("m_nFallbackSeed"), true);
        uintptr_t m_flFallbackWear = kf->off->get(xs("C_EconEntity"), xs("m_flFallbackWear"), true); //m_bAttributesInitialized 
        uintptr_t m_bAttributesInitialized = kf->off->get(xs("C_EconEntity"), xs("m_bAttributesInitialized"));
        if(m_nFallbackPaintKit && m_flFallbackWear && m_nFallbackSeed){
        // kf->Log((handle + m_nFallbackPaintKit), " set pk");
          // *(bool*)(handle + m_bAttributesInitialized) = false;
            *(int32_t*)(handle + m_nFallbackPaintKit) = pk;
            *(int32_t*)(handle + m_nFallbackSeed) = seed;
            *(float*)(handle + m_flFallbackWear) = wear;
           
           // kf->Log("set");
        }
    }
    int32_t getFallBackPaintKit(){
        uintptr_t m_nFallbackPaintKit = kf->off->get(xs("C_EconEntity"), xs("m_nFallbackPaintKit"));
        uintptr_t  m_nFallbackSeed = kf->off->get(xs("C_EconEntity"), xs("m_nFallbackSeed"));
        uintptr_t m_flFallbackWear = kf->off->get(xs("C_EconEntity"), xs("m_flFallbackWear"));
        if(m_nFallbackPaintKit && m_flFallbackWear && m_nFallbackSeed){
             return *(int32_t*)(handle + m_nFallbackPaintKit);
          

        }
    }
    bool prevOwner(){
        uintptr_t m_hPrevOwner = kf->off->get(xs("C_CSWeaponBase"), xs("m_hPrevOwner"));
        if(m_hPrevOwner){
            uint32_t hp = *( uint32_t*)(handle + m_hPrevOwner);
            
            if(hp == 0xffffffff)
                return false;
            else   
                return true;
        }
        return false;
    }
    std::string name(){
        checkIndex();
        if(!itemIndex)
            return xs("BadID");
        return WeaponNameFromID(itemIndex);
    }
    bool IsUnscopedRifleOrSMG()
    {
        checkIndex();
        switch (itemIndex)
        {
        case WEAPON_AK47:
        case WEAPON_M4A1:
        case WEAPON_GALILAR:
        case WEAPON_FAMAS:
        case WEAPON_MAC10:
        case WEAPON_MP5SD:
        case WEAPON_UMP45:
        case WEAPON_BIZON:
        case WEAPON_MP7:
        case WEAPON_MP9:
        case WEAPON_P90:
        case WEAPON_M4_SILENCER:
            return true;
        default:
            return false;
        }
    }
    bool IsPistol()
    {
        checkIndex();
        switch (itemIndex)
        {
        case WEAPON_DESERTEAGLE:
        case WEAPON_ELITES:
        case WEAPON_FIVESEVEN:
        case WEAPON_GLOCK18:
        case WEAPON_TEC9:
        case WEAPON_HKP2000:
        case WEAPON_P250:
        case WEAPON_USP_SILENCER:
        case WEAPON_CZ75:
        case WEAPON_REVOLVER:
            return true;
        default:
            return false;
        }
    }
    bool IsShotgun()
    {
        checkIndex();
        switch (itemIndex)
        {
        case WEAPON_XM1014:
        case WEAPON_MAG7:
        case WEAPON_SAWEDOFF:
        case WEAPON_NOVA:
            return true;
        default:
            return false;
        }
    }
    bool IsScopedRifle()
    {
        checkIndex();
        switch (itemIndex)
        {
        case WEAPON_AUG: //hmm
        case WEAPON_AWP:
        case WEAPON_G3SG1:
        case WEAPON_SCAR20:
        case WEAPON_SG556: //hm
        case WEAPON_SSG08:
            return true;
        default:
            return false;
        }
    }
    bool IsKnife()
    {
        int idx = itemDefinitionIndex();
        switch (idx)
        {
        case WEAPON_KNIFE1:
        case WEAPON_KNIFE:
        case WEAPON_KNIFE2:
        case WEAPON_KNIFE_BUTTERFLY:
        case WEAPON_KNIFECSS:
        case WEAPON_KNIFEKARAM:
        case WEAPON_KNIFEM9:
        case WEAPON_KNIFEBAYONET:
        case WEAPON_KNIFEGUT:
        case WEAPON_KNIFEFLIP:
        case WEAPON_KNIFE_CANIS:
        case WEAPON_KNIFE_CORD:
        case WEAPON_KNIFE_FALCHION_ADVANCED:
        case WEAPON_KNIFE_OUTDOOR:
        case WEAPON_KNIFE_SURVIVAL_BOWIE:
        case WEAPON_KNIFE_GHOST:
        case WEAPON_KNIFE_GYPSY_JACKKNIFE:
        case WEAPON_KNIFE_PUSH:
        case WEAPON_KNIFE_SKELETON:
        case WEAPON_KNIFE_STILETTO:
        case WEAPON_KNIFE_T:
        case WEAPON_KNIFE_URSUS:
        case WEAPON_KNIFE_WIDOWMAKER:
            return true;
        default:
            return false;
        }
    }
    WeaponType type(){
        if(IsScopedRifle())
            return WeaponType::SCOPED;
        else if(IsPistol())
            return WeaponType::PISTOL;
        else if (IsScopedRifle())
            return WeaponType::RIFLESMG;
        else if(IsShotgun())
            return WeaponType::SHOTGUN;
       
        return WeaponType::OTHER;
    }
    static uintptr_t getAddressFromHandle(uintptr_t entity_list, int32_t h){
       
           
            if(h > 0){
                        uintptr_t list_entry2 = *(uintptr_t *)(entity_list + 0x8 * ((h & 0x7FFF) >> 9) + 16);
                         if (list_entry2){
                                 uintptr_t weaponent = *(uintptr_t *)(list_entry2 + 120 * (h & 0x1FF));
                                 if(weaponent)
                                 {
                                    return weaponent;
                                 }
                         }
            }
        
        return 0;
    }
    private:
        uint16_t itemIndex; 
        void checkIndex(){
            if(itemIndex > 0 && valid)
                return;
            itemIndex = itemDefinitionIndex();
            if(!validate())
                return;
            

        }
    public:
        uintptr_t m_Item;
        static uintptr_t m_iItemDefinitionIndex; 
        static uintptr_t m_AttributeManager; 

   
    
    static std::string WeaponNameFromID(uint16_t id)
    {
        switch (id)
        {
        case WEAPON_NONE:
            return xs("NONE");
        case WEAPON_DESERTEAGLE:
            return xs("DEAGLE");
        case WEAPON_ELITES:
            return xs("DUALIES");
        case WEAPON_FIVESEVEN:
            return xs("5/7");
        case WEAPON_GLOCK18:
            return xs("GLOCK");
        case WEAPON_AK47:
            return xs("AK47");
        case WEAPON_AUG:
            return xs("AUG");
        case WEAPON_AWP:
            return xs("AWP");
        case WEAPON_FAMAS:
            return xs("FAMAS");
        case WEAPON_G3SG1:
            return xs("AUTO");
        case WEAPON_GALILAR:
            return xs("GALIL");
        case WEAPON_M249:
            return xs("M249");
        case WEAPON_M4A1:
            return xs("M4A4");
        case WEAPON_MAC10:
            return xs("MAC10");
        case WEAPON_P90:
            return xs("P90");
        case WEAPON_ZONE_REPULSOR:
            return xs("What the Fuck");
        case WEAPON_MP5SD:
            return xs("MP5SD");
        case WEAPON_UMP45:
            return xs("UMP45");
        case WEAPON_XM1014:
            return xs("XM1014");
        case WEAPON_BIZON:
            return xs("BIZON");
        case WEAPON_MAG7:
            return xs("SWAG7");
        case WEAPON_NEGEV:
            return xs("NEGEV");
        case WEAPON_SAWEDOFF:
            return xs("SAWED-OFF");
        case WEAPON_TEC9:
            return xs("TEC9");
        case WEAPON_TASER:
            return xs("ZUES");
        case WEAPON_HKP2000:
            return xs("P2K");
        case WEAPON_MP7:
            return xs("MP7");
        case WEAPON_MP9:
            return xs("MP9");
        case WEAPON_NOVA:
            return xs("NOVA");
        case WEAPON_P250:
            return xs("P250");
        case WEAPON_SHIELD:
            return xs("SHIELD");
        case WEAPON_SCAR20:
            return xs("SCAR20");
        case WEAPON_SG556:
            return xs("SG556");
        case WEAPON_SSG08:
            return xs("SCOUT");
        case WEAPON_KNIFE:
            return xs("KNIFE");
        case WEAPON_KNIFE1:
            return xs("KNIFE");
        case WEAPON_FLASHBANG:
            return xs("FLASH");
        case WEAPON_HE_GRENADE:
            return xs("GRENADE");
        case WEAPON_SMOKE_GRENADE:
            return xs("SMOKE");
        case WEAPON_MOLOTOV:
            return xs("MOLLY");
        case WEAPON_DECOY:
            return xs("DECOY");
        case WEAPON_INCGRENADE:
            return xs("MOLLY");
        case WEAPON_C4:
            return xs("C4");
        case WEAPON_KEVLAR:
            return xs("KEVLAR");
        case WEAPON_ASSAULTSUIT:
            return xs("ASSAULTSUIT");
        case WEAPON_HEAVYASSAULTSUIT:
            return xs("HEAVYASSAULTSUIT");
        case WEAPON_DEFUSER:
            return xs("DEFUSER");
        case WEAPON_CUTTERS:
            return xs("CUTTERS");
        case WEAPON_HEALTHSHOT:
            return xs("HEALTHSHOT");
        case TYPE_MUSICKIT:
            return xs("MUSICKIT");
        case WEAPON_KNIFE_T:
            return xs("KNIFE");
        case WEAPON_M4_SILENCER:
            return xs("M4A1-S");
        case WEAPON_USP_SILENCER:
            return xs("USP");
        case RECIPE_TRADEUP:
            return xs("TRADEUP");
        case WEAPON_CZ75:
            return xs("CZ75");
        case WEAPON_REVOLVER:
            return xs("REVOLVO");
        case WEAPON_TAGRENADE:
            return xs("TAGRENADE");
        case WEAPON_FISTS:
            return xs("FISTS");
        case WEAPON_BREACHCHARGE:
            return xs("BREACHCHARGE");
        case WEAPON_TABLET:
            return xs("TABLET");
        case WEAPON_KNIFE2:
            return xs("KNIFE");
        case WEAPON_AXE:
            return xs("AXE");
        case WEAPON_HAMMER:
            return xs("HAMMER");
        case WEAPON_SPANNER:
            return xs("SPANNER");
        case WEAPON_KNIFE_GHOST:
            return xs("KNIFE");
        case WEAPON_FIREBOMB:
            return xs("FIREBOMB");
        case WEAPON_DIVERSION:
            return xs("DIVERSION");
        case WEAPON_FRAG_GRENADE:
            return xs("FRAG GRENADE");
        case WEAPON_SNOWBALL:
            return xs("SNOWBALL");
        case WEAPON_BUMPMINE:
            return xs("BUMPMINE");
        case WEAPON_KNIFEBAYONET:
            return xs("KNIFE");
        case WEAPON_KNIFECSS:
            return xs("KNIFE");
        case WEAPON_KNIFEFLIP:
            return xs("KNIFE");
        case WEAPON_KNIFEGUT:
            return xs("KNIFE");
        case WEAPON_KNIFEKARAM:
            return xs("KARAMBIT");
        case WEAPON_KNIFEM9:
            return xs("KNIFEM9");
        case WEAPON_KNIFETACTICAL:
            return xs("KNIFE");
        case WEAPON_KNIFE_FALCHION_ADVANCED:
            return xs("KNIFE");
        case WEAPON_KNIFE_SURVIVAL_BOWIE:
            return xs("KNIFE");
        case WEAPON_KNIFE_BUTTERFLY:
            return xs("KNIFE");
        case WEAPON_KNIFE_PUSH:
            return xs("BUTTPLUGS");
        case WEAPON_KNIFE_CORD:
            return xs("KNIFE_CORD");
        case WEAPON_KNIFE_CANIS:
            return xs("KNIFE_CANIS");
        case WEAPON_KNIFE_URSUS:
            return xs("KNIFE_URSUS");
        case WEAPON_KNIFE_GYPSY_JACKKNIFE:
            return xs("KNIFE");
        case WEAPON_KNIFE_OUTDOOR:
            return xs("KNIFE");
        case WEAPON_KNIFE_STILETTO:
            return xs("KNIFE");
        case WEAPON_KNIFE_WIDOWMAKER:
            return xs("KNIFE");
        case WEAPON_KNIFE_SKELETON:
            return xs("KNIFE");
        default:
            return xs("UNKNOWN");
        }
    }
};

//VDATA
    //weapons/models/knife/knife_butterfly/weapon_knife_butterfly.vmdl_c
                     //   const char id[4] = { "507"};
                     //   const char name[32] = { "weapon_knifekaram"};
                     //   uintptr_t VInfo = *(uintptr_t*)(addy + 0x4D8);
                     //   memcpy((void *)(VInfo + 0x250), bknife, sizeof(char) * strlen(bknife) ); 
                    //   memcpy((void *)(VInfo + 0x330), bknife, sizeof(char) * strlen(bknife) ); 
                    //    memcpy((void *)(VInfo + 0x410), bknife, sizeof(char) * strlen(bknife) ); 
                     //   memcpy((void *)(VInfo + 0x30), bknife, sizeof(char) * strlen(bknife) ); 
                    //    uintptr_t id_name = *(uintptr_t*)(VInfo + 0x18);
                     //   uintptr_t name_name = *(uintptr_t*)(VInfo + 0xC18);
                    //    uintptr_t id_id = *(uintptr_t*)(VInfo + 0x10);
                    //    memcpy((void *)(id_name), name, sizeof(char) * 32 ); 
                     //   memcpy((void *)(id_id), id, sizeof(char) * 4 ); 
                    //    memcpy((void *)(name_name), name, sizeof(char) * 32); 
                            //ent + body comp 0x30 + animctl + use client etc
                        //viewmodel 0x7fe86d024000
                        //(viewmodel + bodycomponent) + 0x268 = model name char* 
                        //knife 0x7fe85b3f4800 + m_animationController 0x498 -> + use client side 0x1338
                        // ^ + 0x458 anim related & changing

                        //(weapon + 4D8) + 0x30 = worldmodel name char[161]
                        //(weapon + 4D8) + 0x250 = viewmodel name char[161]
                        //(weapon + 4D8) + 0x330 = playermodel name char[161]
                         //(weapon + 4D8) + 0x410 = worlddropped model name char[161]
                          //(weapon + 4D8) + 0xC18 = item name char* 
                          //(weapon + 4D8) + 0x18 = defidex name char*
                          //(weapon + 4D8) + 0x10 = defidex ID char* 