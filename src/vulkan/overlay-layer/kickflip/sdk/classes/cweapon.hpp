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
        itemIndex = *(uint16_t *)(handle + m_AttributeManager + m_Item + m_iItemDefinitionIndex);
        if(itemIndex > 600 || !itemIndex)
            return 0;
        return itemIndex;
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
    private:
        uint16_t itemIndex; 
        void checkIndex(){
            if(itemIndex > 0 && valid)
                return;
            itemIndex = 0;
            if(!validate())
                return;
            itemDefinitionIndex();

        }
    public:
        uintptr_t m_Item;
        static uintptr_t m_iItemDefinitionIndex; 
        static uintptr_t m_AttributeManager; 

    private:
    
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