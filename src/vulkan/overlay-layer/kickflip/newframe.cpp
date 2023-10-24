#include "includes.hpp"
#include "frame.hpp"
#include "utils.hpp"
#include "offsets.hpp"
#include "cfg/config.hpp"
#include "sdk/sdk.hpp"
#include "sdk/getvar.hpp"
#include "sdk/classes/localplayer.hpp"
#include "schema/dumpschema.hpp"

#include "sdk/classes/plantedC4.hpp"
#include "sdk/classes/cc4.hpp"
#include "sdk/classes/gamerules.hpp"
#include "mem/sigs.hpp"
enum VModes
{
    UN = 0,
    RE,
};

// unsigned __int64 __fastcall CheckInterfacesAndVTables(__int64)
typedef uintptr_t __attribute__((fastcall)) (*CheckInterfacesAndVTablesFn)(uintptr_t);
typedef uintptr_t __attribute__((fastcall)) (*HandleDLLStatusFn)(uintptr_t);
typedef uintptr_t __attribute__((fastcall)) (*UtilCVarCheckFn)(uintptr_t);

int regmod_calls = 0;
int dllstat_calls = 0;
int cvar_calls = 0;
bool ranTest = false;

uintptr_t __attribute__((fastcall)) HandleDLLStatusHk(uintptr_t a1)
{
    dllstat_calls++;
   // kf->menuOpen = true;

    return reinterpret_cast<HandleDLLStatusFn>(kf->DLLStatusHook->getTrampoline())(a1);
}
uintptr_t __attribute__((fastcall)) HandleUtilCvarHk(uintptr_t a1)
{
    cvar_calls++;
   // kf->menuOpen = true;

    return reinterpret_cast<UtilCVarCheckFn>(kf->UtilCVarHook->getTrampoline())(a1);
}

uintptr_t __attribute__((fastcall)) CheckInterfacesAndVTablesHk(uintptr_t a1)
{

    if (!ranTest)
        kf->Log("itf hk test: ");
    uintptr_t ret = 1;
    regmod_calls++;
    kf->menuOpen = true;
    kf->Log(regmod_calls, xs("CheckITFCalled"));
    auto &cl = kf->GetMem()->GetModule(CLIENT_LIBX);

    kf->UnVirtualInsanity(cl, VModes::UN);
    if (ranTest)
        ret = reinterpret_cast<CheckInterfacesAndVTablesFn>(kf->CheckVTablesHook->getTrampoline())(a1); // rip

    kf->UnVirtualInsanity(cl, VModes::RE);
    if (ranTest)
        kf->Log(xs("we did the thing fr oh god"));
    if (!ranTest)
        kf->Log("itf hk test complete-- ");
    return ret;
}
/**
 *  __int64 (__fastcall *__fastcall SetModel(__int64, const char *))() 0x00000B4F680 // E8 ? ? ? ? 49 8B 3F F3 41 0F 10 47 ?
 *
 *
 */

/*
 *  IS_PAINT_KIT_USING_LEGACY_MODEL
 *
 *
 * GET_GC_CLIENT_SYSTEM
 * FIND_SO_CACHE
 *
 *
 *
 * knife:
 * FIRE_EVENT_CLIENT_SIDE
 *
 */
enum frame_stage_t : std::int32_t
{
    FRAME_UNDEFINED = -1,
    FRAME_START,
    FRAME_NET_UPDATE_START,
    FRAME_NET_UPDATE_POSTDATAUPDATE_START,
    FRAME_NET_UPDATE_POSTDATAUPDATE_END,
    FRAME_NET_UPDATE_END,
    FRAME_RENDER_START,
    FRAME_RENDER_END
};
bool fsn_hello = false;
typedef uintptr_t __attribute__((fastcall)) __attribute__((fastcall)) (*SetModelFn)(void *, const char *);

typedef uintptr_t __attribute__((fastcall)) (*FrameStageFn)(uintptr_t, int32_t);

FrameStageFn FrameStageNotifyOg = nullptr;
uintptr_t oldViewmodel = 0;

#define FSN_HOOK
uintptr_t __attribute__((fastcall)) FrameStageNotifyHk(uint64_t a1, int32_t a2)
{
#ifdef FSN_HOOK
    //  kf->DebugState("hello from virtual FSN");
    if (!fsn_hello)
    {
        kf->Log(xs("hello from virtual FSN"));
        uintptr_t m_iItemIDHigh = kf->off->get(xs("C_EconItemView"), xs("m_iItemIDHigh"), true);
        uintptr_t m_iItemIDLow = kf->off->get(xs("C_EconItemView"), xs("m_iItemIDLow"), true);
        fsn_hello = true;
    }
    if (a2 == FRAME_RENDER_END && CEngineClient::Get()->IsInGame()) //
    {

        auto &cl = kf->GetMem()->GetModule(CLIENT_LIBX);
        LocalPlayer lp = LocalPlayer(cl);

        if (lp.validate())
        {
            uintptr_t entity_list = *(uintptr_t *)(cl->start + kf->EntityList());
            if (lp.isAlive())
            {
                CSPlayerPawn localpawn = CSPlayerPawn(lp.pawn(entity_list));
                if (!localpawn.validate())
                    goto ret;
                if (!(lp.team() > 0))
                    goto ret;
                if (!(localpawn.health()))
                    goto ret;
                //  if(!localpawn.clippingWeapon().validate())
                //   goto ret;
                auto weapon_services = localpawn.getWeaponServices();
                uintptr_t m_hMyWeapons = kf->off->get(xs("CPlayer_WeaponServices"), xs("m_hMyWeapons"));
                if (!m_hMyWeapons || !weapon_services)
                    goto ret;

                int num_weapons = *(int *)(weapon_services + m_hMyWeapons); // idiot check
                if (num_weapons < 1)
                {
                    goto ret;
                }
                auto weapons = *(uintptr_t *)(weapon_services + m_hMyWeapons + 0x8);
                std::string weps = std::to_string(num_weapons);
                uintptr_t m_pViewModelServices = kf->off->get(xs("C_CSPlayerPawnBase"), xs("m_pViewModelServices"));

                uintptr_t viewmodelservices = *(uintptr_t *)(localpawn.addy() + m_pViewModelServices);
                 uintptr_t m_pGameSceneNode = kf->off->get(xs("C_BaseEntity"), xs("m_pGameSceneNode"));
                uintptr_t m_hViewModel = kf->off->get(xs("CCSPlayer_ViewModelServices"), xs("m_hViewModel"));

                uint32_t steamID = 1529027502;
                for (int i = 0; i < num_weapons; i++)
                {
                    if (!m_pViewModelServices || !viewmodelservices || !m_hViewModel)
                        continue;
                    uint32_t weaponhandle = *(uint32_t *)(weapons + (0x4 * i));

                    if (weaponhandle == 0)
                        continue;

                    uintptr_t addy = CWeapon::getAddressFromHandle(entity_list, weaponhandle);
                    if (!addy)
                        continue;
                    CWeapon weapon = CWeapon(addy);
                    if (!weapon.validate())
                        continue;
                    /*
                    Weapon Must have GameSceneOrigin + Node To World
                    */
                    if ((uintptr_t *)(weapon.addy() + m_pGameSceneNode) == nullptr)
                        continue;

                    int idx = weapon.itemDefinitionIndex();

                    if (!weapon.IsKnife())
                        continue;

                    const char *karam = "weapons/models/knife/knife_karambit/weapon_knife_karambit.vmdl";
                    const char *butterfly = "weapons/models/knife/knife_butterfly/weapon_knife_butterfly.vmdl";
                    const char *m9 = "weapons/models/knife/knife_m9/weapon_knife_m9.vmdl";
                    const char *gut = "weapons/models/knife/knife_gut/weapon_knife_gut.vmdl";
                    const char *flip = "weapons/models/knife/knife_flip/weapon_knife_flip.vmdl";
                    const char *talon = "weapons/models/knife/knife_talon/weapon_knife_talon.vmdl";
                    const char *plugs = "weapons/models/knife/knife_push/weapon_knife_push.vmdl";
                    char *bknife; // =  "weapons/models/knife/knife_karambit/weapon_knife_karambit.vmdl"; //xc("weapons/models/knife/knife_karambit/weapon_knife_karambit.vmdl"); //xc("weapons/models/knife/knife_butterfly/weapon_knife_butterfly.vmdl");

                    // weapon.setItemDefinitionIndex(WEAPON_KNIFEKARAM);

                    switch (cfg->settings.Skins.knife)
                    {
                    case KnifeGUI::m9:
                        weapon.setItemDefinitionIndex(WEAPON_KNIFEM9);
                        bknife = (char *)m9;
                        break;
                    case KnifeGUI::karam:
                        weapon.setItemDefinitionIndex(WEAPON_KNIFEKARAM);
                        bknife = (char *)karam;
                        break;
                    case KnifeGUI::butt:
                        weapon.setItemDefinitionIndex(WEAPON_KNIFE_BUTTERFLY);
                        bknife = (char *)butterfly;
                        break;
                    case KnifeGUI::flip:
                        weapon.setItemDefinitionIndex(WEAPON_KNIFEFLIP);
                        bknife = (char *)flip;
                        break;
                    case KnifeGUI::gut:
                        weapon.setItemDefinitionIndex(WEAPON_KNIFEGUT);
                        bknife = (char *)gut;
                        break;
                    case KnifeGUI::talon:
                        weapon.setItemDefinitionIndex(523);
                        bknife = (char *)talon;
                        break;
                    case KnifeGUI::buttplug:
                        weapon.setItemDefinitionIndex(WEAPON_KNIFE_PUSH);
                        bknife = (char *)plugs;
                        break;
                    default:
                        weapon.setItemDefinitionIndex(WEAPON_KNIFEKARAM);
                        bknife = (char *)karam;
                        break;
                    }

                    // https://gist.github.com/iamtis/55fb3ca4f9282d42c6b72f8ab5367ee7#pp-bizon
                                                                   //  weapon.setSteamID(steamID);

                    uintptr_t m_CBodyComponent = kf->off->get(xs("C_BaseEntity"), xs("m_CBodyComponent"));
                    uintptr_t m_hWeapon = kf->off->get(xs("C_BaseViewModel"), xs("m_hWeapon"));                                    // class CBaseAnimGraphController  m_animationController
                    uintptr_t m_bClientSideAnimation = kf->off->get(xs("CBaseAnimGraphController"), xs("m_bClientSideAnimation")); // the bar is low
                    uintptr_t m_animationController = kf->off->get(xs("CBodyComponentBaseAnimGraph"), xs("m_animationController"));

                    if (!m_hWeapon || !m_bClientSideAnimation || !m_bClientSideAnimation || !m_CBodyComponent)
                        continue;
                    SetModelFn SetModel = (SetModelFn)(cl->start + kf->SetModel());
                    uint32_t hViewModel = *(uint32_t *)(viewmodelservices + m_hViewModel);

                    uintptr_t viewmodel = 0;
                    if (hViewModel)
                        viewmodel = CWeapon::getAddressFromHandle(entity_list, hViewModel);
                    if (!hViewModel || !viewmodel)
                        continue;

                    //  kf->Log(viewmodel, "viewmodel ent");
                    // kf->Log(addy, "knife ent");

                    uintptr_t kcomp = *(uintptr_t *)(addy + m_CBodyComponent);
                    if (!kcomp)
                        continue;
                    uintptr_t model_nameptr = *(uintptr_t *)(kcomp + 0x268);
                    char model_name[161];
                    strcpy(model_name, (char *)(model_nameptr));

                    //  kf->Log(model_name);
                    if (strcmp(model_name, bknife) != 0)
                    {

                        SetModel((void *)addy, bknife);
                    }
                    uintptr_t vcomp = *(uintptr_t *)(viewmodel + m_CBodyComponent);

                    if (!vcomp)
                        break; //!= nullptr

                    *(uint8_t *)(vcomp + m_animationController + m_bClientSideAnimation) = 0;
                    uintptr_t vmodel_nameptr = *(uintptr_t *)(vcomp + 0x268);
                    char vmodel_name[161];
                    strcpy(vmodel_name, (char *)(vmodel_nameptr));

                    // SetModel((void*)addy, bknife);
                    uint32_t hWeapon = *(uint32_t *)(viewmodel + m_hWeapon);
                    if (!hWeapon)
                        break;
                    if (strcmp(vmodel_name, bknife) == 0 && hWeapon == weaponhandle)
                    {
                        break;
                    }

                    if (viewmodel && hWeapon == weaponhandle)
                        SetModel((void *)viewmodel, bknife); // weapons/models/knife/knife_karambit/weapon_knife_karambit.vmdl_c

                    // kf->Log(weapon.getFallBackPaintKit(), "fbpk");
                    //  kf->Log(weapon.itemDefinitionIndex(), "idx");
                    break;
                }
            }
            // kf->DebugState(weps);
        }
    }

    if (CEngineClient::Get()->IsInGame()) //
    {

        auto &cl = kf->GetMem()->GetModule(CLIENT_LIBX);
        LocalPlayer lp = LocalPlayer(cl);

        if (lp.validate())
        {
            uintptr_t entity_list = *(uintptr_t *)(cl->start + kf->EntityList());
            if (lp.isAlive())
            {
                CSPlayerPawn localpawn = CSPlayerPawn(lp.pawn(entity_list));
                if (!localpawn.validate())
                    goto ret;
                if (!(lp.team() > 0))
                    goto ret;
                if (!(localpawn.health()))
                    goto ret;
                if (!localpawn.clippingWeapon().validate())
                    goto ret;
                auto weapon_services = localpawn.getWeaponServices();
                uintptr_t m_hMyWeapons = kf->off->get(xs("CPlayer_WeaponServices"), xs("m_hMyWeapons"));
                if (!m_hMyWeapons || !weapon_services)
                    goto ret;

                int num_weapons = *(int *)(weapon_services + m_hMyWeapons); // idiot check
                if (num_weapons < 1)
                {
                    goto ret;
                }
                auto weapons = *(uintptr_t *)(weapon_services + m_hMyWeapons + 0x8);
                uintptr_t m_pGameSceneNode = kf->off->get(xs("C_BaseEntity"), xs("m_pGameSceneNode"));
                uintptr_t m_pViewModelServices = kf->off->get(xs("C_CSPlayerPawnBase"), xs("m_pViewModelServices"));
                uintptr_t m_hViewModel = kf->off->get(xs("CCSPlayer_ViewModelServices"), xs("m_hViewModel"));
                uintptr_t viewmodelservices = *(uintptr_t *)(localpawn.addy() + m_pViewModelServices);

                uintptr_t m_hWeapon = kf->off->get(xs("C_BaseViewModel"), xs("m_hWeapon"));
                SetMeshGroupMaskFn SetMeshGroupMask = (SetMeshGroupMaskFn)(cl->start + kf->SetMesh()); // 0C6B810

                for (int i = 0; i < num_weapons; i++)
                {
                    uint32_t hViewModel = *(uint32_t *)(viewmodelservices + m_hViewModel);
                    uintptr_t viewmodel = CWeapon::getAddressFromHandle(entity_list, hViewModel);

                    if (!hViewModel || !viewmodel)
                        continue;
                    uint32_t weaponhandle = *(uint32_t *)(weapons + (0x4 * i));

                    if (weaponhandle == 0)
                        continue;
                   
                    uintptr_t addy = CWeapon::getAddressFromHandle(entity_list, weaponhandle);
                    if (!addy)
                        continue;
                    CWeapon weapon = CWeapon(addy);
                    if (!weapon.validate())
                        continue;
                    /*
                    Weapon Must have GameSceneOrigin + Node To World
                    */

                    if ((uintptr_t *)(weapon.addy() + m_pGameSceneNode) == nullptr)
                        continue;
                    if ((uintptr_t *)(viewmodel + m_pGameSceneNode) == nullptr)
                        continue;

                    uint32_t hWeapon = *(uint32_t *)(viewmodel + m_hWeapon);
                    uintptr_t w_gsn = *(uintptr_t *)(addy + m_pGameSceneNode);
                    uintptr_t v_gsn = *(uintptr_t *)(viewmodel + m_pGameSceneNode);
                    if(weapon.prevOwner())
                        continue;
                    int idx = weapon.itemDefinitionIndex();
                    bool oldModel = true;
                    if(weapon.IsKnife())
                    {
                            weapon.setItemIDHigh();
                            //weapon.setFallBackPaintKit(417, 4, 0.0001f);
                            switch (cfg->settings.Skins.knife)
                            {
                            case KnifeGUI::m9:
                               
                                weapon.setFallBackPaintKit(568, 69, 0.0001f);
                                break;
                            case KnifeGUI::karam:
                             
                                weapon.setFallBackPaintKit(417, 69, 0.0001f); 
                                break;
                            case KnifeGUI::butt:
                                 weapon.setFallBackPaintKit(568, 69, 0.0001f); 
                              
                                break;
                            case KnifeGUI::flip:
                                weapon.setFallBackPaintKit(415, 69, 0.0001f); 
                               
                                break;
                            case KnifeGUI::gut:
                                 weapon.setFallBackPaintKit(565, 69, 0.0001f);
                              
                                break;
                            case KnifeGUI::talon:
                                 weapon.setFallBackPaintKit(856, 69, 0.0001f);
                              
                                break;
                            case KnifeGUI::buttplug:
                                weapon.setFallBackPaintKit(12, 69, 0.0001f);
                               
                                break;
                            default:
                                weapon.setFallBackPaintKit(417, 4, 0.0001f);
                             
                                break;
                            }
                            continue;
                        
                     
                    }
                    // FixupResourceName: Illegal path, missing extension passed in ("CBodyComponentBaseAnimGraphm_animationController"
                    else if (idx == WEAPON_GLOCK18)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(957, 0, 0.0001f);
                        
                    }
                    else if (idx == WEAPON_DESERTEAGLE)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(351, 0, 0.0001f);
                    }
                    else if (idx == WEAPON_USP_SILENCER)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(653, 0, 0.0001f);
                    }
                    else if (idx == WEAPON_ELITES)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(747, 0, 0.0001f);
                    }
                    else if (idx == WEAPON_FIVESEVEN)
                    {
                        weapon.setFallBackPaintKit(427, 0, 0.0001f);
                        weapon.setItemIDHigh();
                    }
                    else if (idx == WEAPON_AK47)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(724, 0, 0.0001f);
                        oldModel = true;
                    }
                    else if (idx == WEAPON_AUG)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(455, 0, 0.0001f);
                    }
                    else if (idx == WEAPON_AWP)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(803, 0, 0.0001f); //344
                        //oldModel = false;
                    }
                    else if (idx == WEAPON_FAMAS)
                    {
                        weapon.setFallBackPaintKit(1127, 0, 0.0001f);
                        weapon.setItemIDHigh();
                    }
                    else if (idx == WEAPON_G3SG1)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(677, 0, 0.0001f);
                    }
                    else if (idx == WEAPON_GALILAR)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(661, 0, 0.0001f);
                    }
                    else if (idx == WEAPON_M249)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(1148, 0, 0.0001f);
                    }
                    else if (idx == WEAPON_M4A1)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(695, 0, 0.0001f); //1228
                        
                    }
                    else if (idx == WEAPON_M4_SILENCER)
                    {
                        weapon.setFallBackPaintKit(984, 0, 0.0001f);
                        weapon.setItemIDHigh();
                       
                    }
                    else if (idx == WEAPON_MAC10)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(1229, 0, 0.0001f);
                    }
                    else if (idx == WEAPON_P90)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(911, 0, 0.0001f);
                    }
                    else if (idx == WEAPON_MP5SD)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(810, 0, 0.0001f);
                    }
                    else if (idx == WEAPON_UMP45)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(1236, 0, 0.0001f);
                    }
                    else if (idx == WEAPON_XM1014)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(557, 0, 0.0001f);
                    }
                    else if (idx == WEAPON_BIZON)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(542, 0, 0.0001f);
                    }
                    else if (idx == WEAPON_MAG7)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(948, 24, 0.0001f);
                    }
                    else if (idx == WEAPON_NEGEV)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(1043, 0, 0.0001f);
                    }
                    else if (idx == WEAPON_SAWEDOFF)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(1155, 0, 0.0001f);
                        weapon.setCustomName("Kiss X Sis");
                    }
                    else if (idx == WEAPON_TEC9)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(791, 0, 0.0001f);
                    }
                    else if (idx == WEAPON_HKP2000)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(389, 0, 0.0001f);
                    }
                    else if (idx == WEAPON_MP7)
                    {
                        weapon.setFallBackPaintKit(1133, 0, 0.0001f);
                        weapon.setItemIDHigh();
                    }
                    else if (idx == WEAPON_MP9)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(1094, 0, 0.0001f);
                    }
                    else if (idx == WEAPON_NOVA)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(306, 0, 0.0001f);
                        oldModel = false;
                    }
                    else if (idx == WEAPON_P250)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(968, 0, 0.0001f);
                    }
                    else if (idx == WEAPON_SCAR20)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(1139, 0, 0.0001f);
                    }
                    else if (idx == WEAPON_SG556)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(38, 0, 0.0001f);
                    }
                    else if (idx == WEAPON_SSG08)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(624, 0, 0.0001f);
                    }
                    else if (idx == WEAPON_CZ75)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(270, 0, 0.0001f);
                    }
                    else if (idx == WEAPON_REVOLVER)
                    {
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(1232, 0, 0.0001f);
                    }
                   
                    if (oldModel == true)
                    {

                        SetMeshGroupMask(w_gsn, 2);

                        if (viewmodel && hWeapon == weaponhandle)
                            SetMeshGroupMask(v_gsn, 2);
                    }
                    else
                    {
                        SetMeshGroupMask(w_gsn, 1);
                        SetMeshGroupMask(v_gsn, 1);
                    }
                }
                // kf->DebugState(weps);
            }
        }
    }
ret:
// return reinterpret_cast<FrameStageFn>(kf->FrameStageHook->getTrampoline())(a1, a2); //rip
#endif
    return FrameStageNotifyOg(a1, a2);
}

void KickFlip::VirtualInsanity(moduleptr_t &cl)
{
    CSource2Client *s2client = CSource2Client::Get(); // ptr to object already allocated
    vtable_ptr = reinterpret_cast<uintptr_t **>(s2client);
    Log(s2client->addy(), "s2cl");
    const uintptr_t start_addr = 0x32F6430;
    const uintptr_t end_addr = 0x032F6960;
    const uintptr_t FSN_idx = 31;
    uintptr_t table_size = (end_addr - start_addr);

    int numVtableEntries = table_size / sizeof(void *);

    Log(numVtableEntries, xs(" V Table Entries "));

    modifiedVTable = new uintptr_t[numVtableEntries];

    memcpy(modifiedVTable, (*vtable_ptr), numVtableEntries * sizeof(uintptr_t));
    Log(modifiedVTable[FSN_idx] - cl->start, xs(" OLD FSN ADDR"));
    FrameStageNotifyOg = reinterpret_cast<FrameStageFn>((*vtable_ptr)[FSN_idx]);
    modifiedVTable[FSN_idx] = reinterpret_cast<uintptr_t>(&FrameStageNotifyHk);
    Log(modifiedVTable[FSN_idx], xs(" NEW FSN ADDR"));
    // Replace the vtable pointer in the object with the modified vtable
    *vtable_ptr = modifiedVTable;

    Log(xs("vtable swap done (WARNING THIS IS DETECTED)"));
    hasHookedFSN = true;
}

void KickFlip::UnVirtualInsanity(moduleptr_t &cl, int mode)
{
    CSource2Client *s2client = CSource2Client::Get(); // ptr to object already allocated
    vtable_ptr = reinterpret_cast<uintptr_t **>(s2client);

    if (mode == VModes::UN)
    {
        *vtable_ptr = (*vtable_ptr);
        Log(xs("restored og vmt"));
    }
    else if (mode == VModes::RE)
    {
        *vtable_ptr = modifiedVTable;
        Log(xs("rehooked vmt"));
         Log(xs("vtable swap test ok (i hope) (WARNING THIS ISNT DETECTED)"));
        fsn_hello = false;
    }

    Log(mode, xs("DID VTABLE SWAP"));
}

void KickFlip::NewFrame(ImGuiContext *imgui_context, float fps, ImVec2 win_size, ImVec2 win_dim)
{
    // traps: VKRenderThread[2124] general protection fault ip:7f044003dbb3 sp:7f03a2bfd1c0 error:0 in libflipover.so (deleted)[7f0440026000+a0000]
    //  ImGui::SetCurrentContext(imgui_context);

    // ADD A CHECK IF LIB WASNT DELETED AND CRASH GAME IF IT WASNT

    if (!cfg->isLoaded())
    {
        Log(cfg->Init(), (uint8_t)LOG_INFO);
        menuOpen = cfg->settings.KF.LaunchMenu; // badddd
        // version = cfg->settings.VERSION;
        // assert(cfg->isLoaded());
    }
    if (in != nullptr)
    {
        if (in->hasInitialized())
        {
            ImGuiIO &io = ImGui::GetIO();
            io.MousePos = GetInput()->MousePos();
            io.MouseDown[0] = GetInput()->Mouse(1);
            io.MouseDown[1] = GetInput()->Mouse(2);
            if (GetInput()->MenuKey())
            {
                if (stateChangeLastFrame == false)
                {
                    menuOpen = !menuOpen;
                    stateChangeLastFrame = true;
                }
            }
            else
            {
                stateChangeLastFrame = false;
            }
        }
    }
    else
    {
        in = new InputManager();
    }

    if (!bounds.x)
    {
        bounds.x = win_dim.x; // draw_data->DisplaySize.x
        bounds.y = win_dim.y; // if w2s broke this is why
        aim->setBounds(bounds);
    }
    ImGuiWindowFlags flags = (menuOpen) ? 0 : ImGuiWindowFlags_NoMouseInputs;
    bool overlay = true;

    //  if(cfg->js["KF"]["FPS"].ToBool())
    char buf[128];
    sprintf(buf, "Kickflip %s [%s] %.0f###KFMain", version.c_str(), (status ? "ok" : "bad"), fps);
    const float margin = 10.0f;
    if (!menuOpen)
    {
        ImGui::SetNextWindowSize(win_size, ImGuiCond_Always); // ahhh
        ImGui::SetNextWindowPos(ImVec2(win_dim.x - win_size.x - margin, margin + 10),
                                ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.5);
        ImGui::SetNextWindowCollapsed(true);
    }
    else
    {
        ImGui::SetNextWindowSize(ImVec2(win_size.x * 4, win_size.y * 3), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(win_dim.x - (win_size.x * 4) - margin, margin + 10),
                                ImGuiCond_Always); // AHAHHAHAHH
        ImGui::SetNextWindowBgAlpha(0.8);
        ImGui::SetNextWindowCollapsed(false);
    }
    // if(!gui.hasStyle)
    //   gui.setStyle(); //? why no persist FUGLY
    ImGui::Begin(buf, &overlay, flags);
    if (menuOpen)
    {
        gui.drawTabBar(xc("Home"), 0);
        gui.drawTabBar(xc("ESP"), 1);
        gui.drawTabBar(xc("Dev"), 2);
        gui.drawTabBar(xc("Aim"), 3);
        gui.drawTabBar(xc("Save"), 4);
        gui.drawTabBar(xc("Close Menu"), 5);
        switch (gui.currentTab)
        {
        case 0:
            gui.drawMainTab();
            break;
        case 1:
            gui.drawESPTab();
            break;
        case 2:
            gui.drawDevTab();
            break;
        case 3:
            gui.drawAimTab();
            break;
        case 4:
            cfg->save();
            gui.currentTab = 0;
            break;
        case 5:
            menuOpen = false;
            gui.currentTab = 0;
            break;
        default:
            gui.drawMainTab();
        }
#ifdef DEVTOOLS
        ImGui::Text("DevTools Enabled - not safe");
        if (ImGui::Button("crash game"))
        {
        }
        ImGui::SameLine();
        if (ImGui::Button("Dump To Desktop"))
        {
            DevTools::DumpToFile("/home/dylan/Desktop/cs2client1019.txt");
        }
#endif

        // auto &cl = GetMem()->GetModule(CLIENT_LIBX);
        // typedef int64_t (*HandleInvFn)();   //00652B50   55 31 D2 31 F6 48 89 E5 41 56 41 55 4C 8D AD ? ? ? ? HandleInventoryResponse
        //  HandleInvFn hi = (HandleInvFn)(cl->start + 0x00652B50);
        // CheckInterfacesAndVTablesFn CheckInterfacesAndVTablesOg = (CheckInterfacesAndVTablesFn)(cl->start + 0x0650F60);

        // Log("calling");
        // CheckInterfacesAndVTablesOg((uintptr_t)data);

        // hi();

        ImGui::Text(hasHookedFSN ? xc("Unsafe Mode") : xc("Safe Mode"));
        ImGui::Text(__DATE__);

        ImGui::Text(__TIME__);
    }

    ImGui::End();

    uintptr_t client_start = 0;

    if (ok && GetMem()->Ready())
    {
        auto &cl = GetMem()->GetModule(CLIENT_LIBX);
        if (cl)
            client_start = cl->start;
        if (!hasHookedFSN && CSource2Client::Get()->addy())
        {

            VirtualInsanity(cl);
            // E8 ? ? ? ? 31 D2 31 F6 4C 89 EF E8 ? ? ? ? 4C 89 E2
            CheckInterfacesAndVTablesFn CheckInterfacesAndVTablesOg = (CheckInterfacesAndVTablesFn)(client_start + kf->VMTCheck()); // 10.20
            CheckVTablesHook = new DetourHooking::Hook(reinterpret_cast<void *>(CheckInterfacesAndVTablesOg), reinterpret_cast<void *>(CheckInterfacesAndVTablesHk), 8);
            CheckVTablesHook->enable();
            if (CheckVTablesHook->getError() == DetourHooking::Error::SUCCESS)
            {
                Log("VTB :))");
                ranTest = CheckInterfacesAndVTablesOg(7);

                Log(ranTest, "Test Result: ");
                ranTest = true;
            }
            /*
            HandleDLLStatusFn HandleDLLOg = (HandleDLLStatusFn)(client_start + 0x00650A40); // 10.20
            DLLStatusHook = new DetourHooking::Hook(reinterpret_cast<void *>(HandleDLLOg), reinterpret_cast<void *>(HandleDLLStatusHk), 8);
            DLLStatusHook->enable();
            if (DLLStatusHook->getError() == DetourHooking::Error::SUCCESS)
                Log("dllstat hk ok");
            UtilCVarCheckFn CvarOg = (UtilCVarCheckFn)(client_start + 0x0652B20); // 10.20
            UtilCVarHook = new DetourHooking::Hook(reinterpret_cast<void *>(CvarOg), reinterpret_cast<void *>(HandleUtilCvarHk), 8);
            UtilCVarHook->enable();
            if (UtilCVarHook->getError() == DetourHooking::Error::SUCCESS)
                Log("util hk ok");
            */
            // HandleDLLStatus 48 8B 47 18 48 83 E0 FC 48 8B 38  //0x00650A40
            //__int64 __fastcall HandleDLlStatus(__int64)

            //__int64 __fastcall CUserMessageRequestUtilAction_iFunc(__int64 a1) 55 44 8B 4F 18 oh boy  //0x0652B20

            // Log(CheckInterfacesAndVTablesOg((uintptr_t) *data), "ticks");
            // Log(data);

            // FrameStageFn FrameStageNotify = (FrameStageFn)(client_start + 0x0D41910); // 55 48 89 E5 41 55 41 54 49 89 FC 53 89 F3 48 83 EC 08 39 1D ? ? ? ?
            // FrameStageHook = new DetourHooking::Hook(reinterpret_cast<void *>(FrameStageNotify), reinterpret_cast<void *>(FrameStageNotifyHk), 8);
            //  FrameStageHook->enable();
            //  if (FrameStageHook->getError() == DetourHooking::Error::SUCCESS)
            //     Log("FSN :))");

            hasHookedFSN = true;
        }
    }

    status = (client_start > 0);

    bool confidentlyInGame = false; // TIME TO LOOK AT ENGINE
    int minfps = 100;
    if (cfg->settings.ESP.FPSLimit > 80)
        minfps = cfg->settings.ESP.FPSLimit;
    if (client_start != 0 && fps > minfps)
    { // bad
        std::string hks = "vmt: " + std::to_string(regmod_calls) + " dll: " + std::to_string(dllstat_calls) + " cvar: " + std::to_string(cvar_calls);
        DebugState(hks);
        auto &cl = GetMem()->GetModule(CLIENT_LIBX);
        if (!cl)
            goto null;

        LocalPlayer lp = LocalPlayer(cl);

        if (!lp.validate() || !CEngineClient::Get()->IsInGame())
        {

            // ImGui::Text("loading...");
            goto null;
        }
        uintptr_t entity_list = *(uintptr_t *)(client_start + EntityList());
        if (entity_list)
        {

            //  CSGameRules *gamerules = (CSGameRules *)(client_start + GameRules());
            // if (gamerules != nullptr)
            // {
            //     if (gamerules->m_bTeamIntroPeriod())

            // }
            view_matrix_t matrix = *(view_matrix_t *)(client_start + ViewMatrix());
            bool isSomeoneDefusing = false;

            auto c4 = PlantedC4();
            auto weaponc4 = CC4();
            if (c4.ok() && c4.isPlanted() && cfg->settings.ESP.BombTimer)
            {
                if (c4.exists()) //|| true
                {
                    float time = c4.TimeRemaining();
                    if (time >= 0.f)
                    {
                        Vector3 C4origin = c4.origin();
                        C4origin.z += 10.f;
                        Vector3 sC4 = Util::WorldToScreen(bounds, C4origin, matrix);
                        if (sC4.z > 0.01f)
                        {
                            if (c4.defusing() && !c4.exploded())
                            {
                                float defuse_t = c4.defuselength() * -1.f; // magic
                                char tr[5];
                                sprintf(tr, "%.1f", defuse_t);
                                ImVec4 col = ImVec4(0, 0.3, 0.9, 1);
                                if (c4.defusingwithkit())
                                    col = ImVec4(1, 0, 1, 1);
                                ImGui::GetOverlayDrawList()->AddText(40.f, ImVec2(sC4.x, sC4.y - 60.f), ImGui::ColorConvertFloat4ToU32(col), tr);
                            }
                            if (!c4.defused() && !c4.exploded())
                            {
                                char tr[5];
                                sprintf(tr, "%.1f", time);

                                ImGui::GetOverlayDrawList()->AddText(40.f, ImVec2(sC4.x, sC4.y), ImGui::ColorConvertFloat4ToU32((time > 10.f) ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1)), tr);
                            }
                        }
                    }
                }

            } // end if plantedc4
            else if (!c4.isPlanted() && cfg->settings.ESP.Labels.C4)
            {
                if (weaponc4.ok() && weaponc4.inPlay())
                {
                    Vector3 og = weaponc4.origin();
                    uintptr_t ownerHandle = weaponc4.hOwnerEntity();

                    if (weaponc4.dropped())
                    {
                        Vector3 wC4 = Util::WorldToScreen(bounds, og, matrix);
                        if (wC4.z > 0.01f)
                        {
                            ImGui::GetOverlayDrawList()->AddText(30.f, ImVec2(wC4.x, wC4.y), ImGui::ColorConvertFloat4ToU32(ImVec4(0.7f, 0.2f, 0.4f, 1.f)), xc("C4"));
                        }
                    }
                }

            } // end carried C4 */
            bool foundC4 = false;
            matrix = *(view_matrix_t *)(client_start + ViewMatrix());
            for (int i = 1; i < ENT_MAX; i++)
            {

                if (!lp.validate())
                    continue;
                PlayerController player = PlayerController(PlayerController::GetPlayerFromIndex(entity_list, i));
                if (!player.validate())
                    continue;

                if (player.addy() == lp.addy())
                    continue;

                if (!player.isAlive())
                    continue;
                if (player.isLocalPlayerController())
                    continue;

                int team = player.team();

                if (team < Teams::ALL)
                    continue;
                if (team > Teams::CT)
                    continue;

                bool label = true;
                bool teammate = false;
                if (team == lp.team())
                {
                    teammate = true;
                    if (!lp.isAlive())
                        continue;
                    else if (cfg->settings.ESP.Labels.OnlyLabelEnemy)
                    {
                        label = false;
                    }
                }

                std::string name;

                if (cfg->settings.ESP.Labels.Name && label)
                    name = player.name();

                bool isDude = false;
                if (cfg->settings.ESP.Bones.Cocks && teammate)
                {
                    if (name.find(xs("dude")) != std::string::npos && name.find("car") != std::string::npos)
                        isDude = true;
                    else if (name.find(xs("spectator")) != std::string::npos)
                        isDude = true;
                }

                CSPlayerPawn pawn = CSPlayerPawn(player.pawn(entity_list));

                if (!pawn.validate())
                    continue;
                if (lp.isAlive())
                {
                    CSPlayerPawn localpawn = CSPlayerPawn(lp.pawn(entity_list));
                    if (localpawn.addy() == pawn.addy())
                        continue;
                }

                int health = pawn.health();

                if (!(health >= 0) || (health > 100) || health == 0) // bad
                    continue;

                if (!pawn.movedSinceSpawn())
                    label = false;
                std::string WeaponName = "None";
                if (cfg->settings.ESP.Labels.Weapon && label)
                {
                    CWeapon weapon = pawn.clippingWeapon();
                    if (weapon.validate())
                    {
                        uint16_t idx = weapon.itemDefinitionIndex();
                        WeaponName = CWeapon::WeaponNameFromID(idx);
                    }
                }
                bool hasC4 = false;
                if (!c4.isPlanted() && cfg->settings.ESP.Labels.C4 && weaponc4.exists() && !foundC4)
                {
                    if (!weaponc4.dropped())
                    {
                        if (uintptr_t owner = weaponc4.getOwnerAddress(entity_list); owner > 0)
                        {
                            if (owner == pawn.addy())
                            {

                                hasC4 = true;
                                foundC4 = true;
                            }
                        }
                    }
                }
                uint8_t is_defusing = 0;
                if (team == 3)
                {
                    is_defusing = pawn.defusing(); // if always true its broken
                    if (is_defusing)
                        isSomeoneDefusing = true;
                }

                Vector3 origin = pawn.oldOrigin();
                if (origin.x == 0.f || origin.y == 0.f || origin.z == 0.f)
                    continue;

                Vector3 head;
                head.x = origin.x;
                head.y = origin.y;
                head.z = origin.z + 75.f; // -60
                Vector3 screen = Util::WorldToScreen(bounds, origin, matrix);
                Vector3 screenhead = Util::WorldToScreen(bounds, head, matrix);

                //

                if (screen.z < 0.01f)
                    continue;

                float h = screen.y - screenhead.y;
                float w = h / 2.4f;
                float x = screenhead.x - w / 2;
                float y = screenhead.y;
                if (w > (0.4f * bounds.x))
                    continue;

                if (((y + y + h) < 5))
                    continue;
                ImVec4 box_color = ImVec4(1, 1, 1, 1);

                bool boxes, lines;
                boxes = true;
                lines = false;
                if (team == 3 && is_defusing)
                    box_color = cfg->settings.ESP.Box.DColor;
                else if (team == 3 && !is_defusing)
                    box_color = cfg->settings.ESP.Box.CTColor; // 155
                else if (team == 2)
                    box_color = cfg->settings.ESP.Box.TColor;
                if (cfg->settings.ESP.Box.DrawEnemy && !teammate)
                {
                    // x, y            x + w, y + h  //int x, int y, int w, int h,
                    ImGui::GetOverlayDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(box_color), 0.2, 0, 1); // thickness
                }
                else if (cfg->settings.ESP.Box.DrawTeam && teammate)
                {
                    // x, y            x + w, y + h  //int x, int y, int w, int h,
                    ImGui::GetOverlayDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(box_color), 0.2, 0, 1); // thickness
                }

                if (cfg->settings.ESP.Labels.HP && label)
                {
                    std::string utf_8_1 = std::to_string(health); // fuck utf
                    float hp_mod = (health * 0.01);
                    ImVec4 clr = (cfg->settings.ESP.Labels.AlterColor) ? ImVec4(1, hp_mod, hp_mod, 1) : cfg->settings.ESP.Labels.Color;
                    ImGui::GetOverlayDrawList()->AddText(15.f, ImVec2(x + w, y), ImGui::ColorConvertFloat4ToU32(clr), utf_8_1.c_str());
                }

                if (cfg->settings.ESP.Labels.Name && label)
                {

                    ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y + 12.f), ImGui::ColorConvertFloat4ToU32(cfg->settings.ESP.Labels.Color), name.c_str());
                }
                if (cfg->settings.ESP.Labels.C4 && hasC4 && label)
                {

                    ImGui::GetOverlayDrawList()->AddText(15.f, ImVec2(x, y - 5.f), ImGui::ColorConvertFloat4ToU32(cfg->settings.ESP.Labels.Color), xc("C4"));
                }

                if (cfg->settings.ESP.Labels.Weapon && label)
                {
                    ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y + h + 5.f), ImGui::ColorConvertFloat4ToU32(cfg->settings.ESP.Labels.Color), WeaponName.c_str());
                }
                if (cfg->settings.ESP.Bones.DrawTeam && teammate)
                {

                    esp.DrawBones(pawn.addy(), bounds, matrix, isDude); // isDude
                    // StickFigure(CSPlayerPawn, bounds, matrix, team, is_defusing);
                    // esp.OldDrawBones(pawn.addy(), lp.addy(), bounds, matrix, team, is_defusing);
                }
                if (cfg->settings.ESP.Bones.DrawEnemy && !teammate)
                {

                    esp.DrawBones(pawn.addy(), bounds, matrix, false); // isDude
                                                                       //  StickFigure(CSPlayerPawn, bounds, matrix, team, is_defusing);
                    // esp.OldDrawBones(pawn.addy(), lp.addy(), bounds, matrix, team, is_defusing);
                }
                confidentlyInGame = true;
                // https://www.unknowncheats.me/forum/general-programming-and-reversing/432465-imgui-drawing.html
                // https://github.com/UnnamedZ03/CS2-external-base/blob/main/source/Source.cpp
            } // end entlist iterate

            if (cfg->settings.AIM.drawFOV && cfg->settings.AIM.Enabled)
                ImGui::GetOverlayDrawList()->AddCircle(ImVec2(bounds.x / 2, bounds.y / 2), cfg->settings.AIM.fov / 2, ImGui::ColorConvertFloat4ToU32(ImVec4(0.8, 0.1, 0.1, 0.7)));
        } // end if entlist
    }
    goto null;
null:
}

void KickFlip::SigScan()
{
    sig_attempts++;
    if (foundOffsets == true)
        return;

    if (sig_attempts > 3)
    {
        //Log(xs("SIG SCAN FAILURE - CRITICAL"));
        return;
    }
    sigs s = sigs();
    LocalPlayerOffset = s.getLocalPlayerOffset();
    EntListOffset = s.getEntityListOffset();
    TraceShapeOffset = s.getTraceShapeOffset();
    EngineTracePtrOffset = s.getEngineTraceOffset();
    ViewMatrixOffset = s.getViewMatrixOffset();
    PlantedC4Offset = s.getPlantedC4Offset();
    SetModelFnOffset = s.getSetModelOffset();
    WeaponC4Offset = s.getWeaponC4Offset();
    SetMeshOffset = s.getSetMeshOffset();
    VMTOffset = s.getVMTOffset();

    if (!LocalPlayerOffset || !EntListOffset || !TraceShapeOffset || !EngineTracePtrOffset || !ViewMatrixOffset || !PlantedC4Offset || !WeaponC4Offset || !SetModelFnOffset  ) //!SetMeshOffset || !VMTOffset
    {
        foundOffsets = false;
        
        Log(xs("SIG SCAN FAILURE - CRITICAL"));
        return;
    }
    Log(xs("Sig. Scan Success"));
    foundOffsets = true;
    return;
}
