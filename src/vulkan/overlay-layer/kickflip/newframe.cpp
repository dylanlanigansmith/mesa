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


/**
 *  __int64 (__fastcall *__fastcall SetModel(__int64, const char *))() 0x00000B4F680 // E8 ? ? ? ? 49 8B 3F F3 41 0F 10 47 ? 
 * 
 * 
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
typedef uintptr_t __attribute__((fastcall)) __attribute__((fastcall)) (*SetModelFn)(void*, const char*);


typedef uintptr_t __attribute__((fastcall)) (*FrameStageFn)(uintptr_t, int32_t);

FrameStageFn FrameStageNotifyOg = nullptr;
uintptr_t oldViewmodel = 0;
uintptr_t __attribute__((fastcall)) FrameStageNotifyHk(uint64_t a1, int32_t a2)
{

  //  kf->DebugState("hello from virtual FSN");
    if (a2 == FRAME_RENDER_END && CEngineClient::Get()->IsInGame())
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
                if(!(localpawn.health()))
                    goto ret;
                // if(!localpawn.clippingWeapon().validate())
                //   goto null;
                auto weapon_services = localpawn.getWeaponServices();
                uintptr_t m_hMyWeapons = kf->off->get(xs("CPlayer_WeaponServices"), xs("m_hMyWeapons"));
                if (!m_hMyWeapons)
                    goto ret;

                int num_weapons = *(int *)(weapon_services + m_hMyWeapons); // idiot check

                auto weapons = *(uintptr_t *)(weapon_services + m_hMyWeapons + 0x8);
                std::string weps = std::to_string(num_weapons);
                 uintptr_t m_pViewModelServices = kf->off->get(xs("C_CSPlayerPawnBase"), xs("m_pViewModelServices"));   
                                           
                        uintptr_t viewmodelservices = *(uintptr_t*)(localpawn.addy() + m_pViewModelServices);  

                        uintptr_t m_hViewModel = kf->off->get(xs("CCSPlayer_ViewModelServices"), xs("m_hViewModel"));   

                        uint32_t hViewModel = *(uint32_t*)(viewmodelservices + m_hViewModel);

                        uintptr_t viewmodel = CWeapon::getAddressFromHandle(entity_list, hViewModel);
                for (int i = 0; i < num_weapons; i++)
                {
                    uint32_t weaponhandle = *(uint32_t *)(weapons + (0x4 * i) );
                  
                    if (weaponhandle == 0)
                        continue;

                    uintptr_t addy = CWeapon::getAddressFromHandle(entity_list, weaponhandle);
                    if (!addy)
                        continue;
                    CWeapon weapon = CWeapon(CWeapon::getAddressFromHandle(entity_list, weaponhandle));
                   
                   /*
                   Weapon Must have GameSceneOrigin + Node To World 
                   */
                    if((uintptr_t*)(weapon.addy() + 0x488) == nullptr)
                       continue;
                    uint32_t steamID = 1010697080; //steamid3
                    int idx = weapon.itemDefinitionIndex();
              
                     std::string n = CWeapon::WeaponNameFromID(idx);
                    weps += (n + " " + std::to_string(idx));

                    if(oldViewmodel != 0 && idx != WEAPON_KNIFE_BUTTERFLY)
                        *(uintptr_t*)(viewmodel + 0x30)  = oldViewmodel;
                   //  kf->Log(addy , std::to_string(idx) + " w ptr" + n);
                    if (idx == WEAPON_KNIFE || idx == WEAPON_KNIFE_T || idx == WEAPON_KNIFE_BUTTERFLY || idx ==  WEAPON_KNIFE1)
                    {
                         weapon.setSteamID(steamID);
                          weapon.setItemIDHigh();
                        weapon.setItemDefinitionIndex(WEAPON_KNIFE_BUTTERFLY);
                        weapon.setFallBackPaintKit(38, 0, 0.001f); // https://gist.github.com/iamtis/55fb3ca4f9282d42c6b72f8ab5367ee7#pp-bizon
                       
                       
                        uintptr_t m_hWeapon  = kf->off->get(xs("C_BaseViewModel"), xs("m_hWeapon"));    //class CBaseAnimGraphController  m_animationController
                        uintptr_t  m_bClientSideAnimation = kf->off->get(xs("CBaseAnimGraphController"), xs("m_bClientSideAnimation"));   //the bar is low
                         uintptr_t  m_animationController = kf->off->get(xs("CBodyComponentBaseAnimGraph"), xs("m_animationController"));
                      //  *(bool*)(viewmodel + m_bClientSideAnimation) = false;
                      kf->Log(viewmodel, "viewmodel ent");
                      kf->Log(addy, "knife ent"); 
                        
                        uintptr_t animctl = *(uintptr_t*)(viewmodel + m_animationController);
                        if(animctl){ //!= nullptr
                            *(uint8_t*)(animctl + m_bClientSideAnimation) = 0;
                            
                        }
                        animctl = *(uintptr_t*)(addy + m_animationController);
                        if(animctl ){
                            *(uint8_t*)(animctl + m_bClientSideAnimation) = 0;
                          
                        }
                        const char* bknife =  "weapons/models/knife/knife_butterfly/weapon_knife_butterfly.vmdl"; //weapons/models/knife/knife_butterfly/weapon_knife_butterfly.vmdl_c
                        const char id[4] = { "515"};
                        const char name[32] = { "weapon_knife_butterfly"};
                        uintptr_t VInfo = *(uintptr_t*)(addy + 0x4D8);
                        memcpy((void *)(VInfo + 0x250), bknife, sizeof(char) * strlen(bknife) ); 
                        memcpy((void *)(VInfo + 0x330), bknife, sizeof(char) * strlen(bknife) ); 
                        memcpy((void *)(VInfo + 0x410), bknife, sizeof(char) * strlen(bknife) ); 
                        memcpy((void *)(VInfo + 0x30), bknife, sizeof(char) * strlen(bknife) ); 
                        uintptr_t id_name = *(uintptr_t*)(VInfo + 0x18);
                        uintptr_t name_name = *(uintptr_t*)(VInfo + 0xC18);
                        uintptr_t id_id = *(uintptr_t*)(VInfo + 0x10);
                        memcpy((void *)(id_name), name, sizeof(char) * 32 ); 
                        memcpy((void *)(id_id), id, sizeof(char) * 4 ); 
                        memcpy((void *)(name_name), name, sizeof(char) * 32); 
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


                        uint32_t hWeapon = *(uint32_t*)(viewmodel + m_hWeapon);
                        SetModelFn SetModel = (SetModelFn)(cl->start + 0x00000B4F680); 
                        
                        SetModel((void*)addy, bknife);

                        uintptr_t holyfuck = *(uintptr_t*)(addy + 0x30);
                        if(oldViewmodel == 0)
                            oldViewmodel = *(uintptr_t*)(viewmodel + 0x30) ;
                       
                        if(viewmodel && hWeapon == weaponhandle)
                            SetModel((void*)viewmodel, bknife); //weapons/models/knife/knife_karambit/weapon_knife_karambit.vmdl_c
                       //  kf->Log(weapon.getItemIDHigh(), "item hi");

                        *(uintptr_t*)(viewmodel + 0x30) = holyfuck;
                        
                         //kf->Log(weapon.getFallBackPaintKit(), "fbpk");
                        continue;
                    }
                 
                    else if(idx ==  WEAPON_GLOCK18){
                        weapon.setItemIDHigh();
                        weapon.setFallBackPaintKit(38, 69, 0.001f);
                        //weapon.setSteamID(steamID);
                        weapon.setCustomName("lmao");
                        kf->Log(weapon.getFallBackPaintKit(), "fbpk");
                        kf->Log(weapon.getItemIDHigh(), "item hi");
                        continue;
                    }
                     else if(idx ==  WEAPON_AK47){
                        weapon.setFallBackPaintKit(490, 0, 0.001f);
                       //  weapon.setSteamID(steamID);
                        weapon.setItemIDHigh();
                        continue;
                    }
                    else if(idx == WEAPON_AWP){
                        weapon.setFallBackPaintKit(279, 0, 0.1f);
                        weapon.setSteamID(steamID);
                        weapon.setCustomName("lmao");
                        weapon.setItemIDHigh();
                        continue;
                    }
                }
                kf->DebugState(weps);
            }
        }
    }
ret:
   // return reinterpret_cast<FrameStageFn>(kf->FrameStageHook->getTrampoline())(a1, a2); //rip

   return FrameStageNotifyOg(a1, a2);
}


void KickFlip::VirtualInsanity(moduleptr_t &cl)
{
     CSource2Client* s2client = CSource2Client::Get(); //ptr to object already allocated 
     vtable_ptr = reinterpret_cast<uintptr_t**>(s2client);
    Log(s2client->addy(), "s2cl");
     const uintptr_t start_addr = 0x32F6430;
      const uintptr_t end_addr = 0x032F6960;
      const uintptr_t FSN_idx = 31;
    uintptr_t table_size = (end_addr - start_addr);

    int numVtableEntries = table_size / sizeof(void*);


    Log(numVtableEntries, xs(" V Table Entries "));
  


    modifiedVTable = new uintptr_t[numVtableEntries];

    memcpy(modifiedVTable, (*vtable_ptr), numVtableEntries * sizeof(uintptr_t));
    Log(modifiedVTable[FSN_idx] - cl->start, xs(" OLD FSN ADDR"));
    FrameStageNotifyOg = reinterpret_cast<FrameStageFn>((*vtable_ptr)[FSN_idx]);
    modifiedVTable[FSN_idx] = reinterpret_cast<uintptr_t>(&FrameStageNotifyHk);
    Log(modifiedVTable[FSN_idx], xs(" NEW FSN ADDR"));
    // Replace the vtable pointer in the object with the modified vtable
    *vtable_ptr = modifiedVTable;

      
    Log(xs("we did something idk if it worked"));

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

        if (ImGui::Button("crash game"))
        {
           
            
        }

        if (ImGui::Button("close"))
        {
            menuOpen = false;
        }
        ImGui::Text(hasHookedFSN ? xc("UNSAFE!!!") : xc("SAFE"));
    }

    ImGui::End();

    uintptr_t client_start = 0;

    if (ok && GetMem()->Ready())
    {
        auto &cl = GetMem()->GetModule(CLIENT_LIBX);
        if (cl)
            client_start = cl->start;
        if (!hasHookedFSN)
        {

            //FrameStageFn FrameStageNotify = (FrameStageFn)(client_start + 0x0D41910); // 55 48 89 E5 41 55 41 54 49 89 FC 53 89 F3 48 83 EC 08 39 1D ? ? ? ?
           // FrameStageHook = new DetourHooking::Hook(reinterpret_cast<void *>(FrameStageNotify), reinterpret_cast<void *>(FrameStageNotifyHk), 8);
          //  FrameStageHook->enable();
          //  if (FrameStageHook->getError() == DetourHooking::Error::SUCCESS)
           //     Log("FSN :))");

           
            VirtualInsanity(cl);

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

            CSGameRules *gamerules = (CSGameRules *)(client_start + GameRules());
            if (gamerules != nullptr)
            {
                if (gamerules->m_bTeamIntroPeriod())
                {
                    //  Log("INTROOOOOOOOOO");
                    // goto null;
                }
            }
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
                        weapon.itemDefinitionIndex();
                        WeaponName = weapon.name();
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

    if (foundOffsets == true)
        return;
    sigs s = sigs();
    LocalPlayerOffset = s.getLocalPlayerOffset();
    EntListOffset = s.getEntityListOffset();
    TraceShapeOffset = s.getTraceShapeOffset();
    EngineTracePtrOffset = s.getEngineTraceOffset();
    ViewMatrixOffset = s.getViewMatrixOffset();
    PlantedC4Offset = s.getPlantedC4Offset();

    if (!LocalPlayerOffset || !EntListOffset || !TraceShapeOffset || !EngineTracePtrOffset || !ViewMatrixOffset || !PlantedC4Offset)
    {
        foundOffsets = false;
        Log(xs("SIG SCAN FAILURE - CRITICAL"));
        return;
    }
    Log(xs("Sig. Scan Success"));
    foundOffsets = true;
    return;
}
