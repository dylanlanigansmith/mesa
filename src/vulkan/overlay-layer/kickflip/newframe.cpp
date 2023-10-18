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
#include "mem/sigs.hpp"
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

         if(ImGui::Button("crash game")){

           Log(kf->off->get(xs("CBasePlayerController"),xs("m_bIsLocalPlayerController")), "isLPC");
          }
           
        if (ImGui::Button("close"))
        {
            menuOpen = false;
        }
    }

    ImGui::End();

    uintptr_t client_start = 0;

    if (ok && GetMem()->Ready())
    {
        auto &cl = GetMem()->GetModule(CLIENT_LIBX);
        if (cl)
            client_start = cl->start;
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

            view_matrix_t matrix = *(view_matrix_t *)(client_start + ViewMatrix());

            bool isSomeoneDefusing = false;
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
                if(player.isLocalPlayerController())
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
                 if(lp.isAlive()){
                         CSPlayerPawn localpawn = CSPlayerPawn(lp.pawn(entity_list));
                         if(localpawn.addy() == pawn.addy())
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
                if (cfg->settings.ESP.Labels.Weapon && label)
                {
                    ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y + h + 5.f), ImGui::ColorConvertFloat4ToU32(cfg->settings.ESP.Labels.Color), WeaponName.c_str());
                }
                if (cfg->settings.ESP.Bones.DrawTeam && teammate)
                {
                    esp.DrawBones(pawn.addy(), bounds, matrix, isDude); //isDude
                    // StickFigure(CSPlayerPawn, bounds, matrix, team, is_defusing);
                   // esp.OldDrawBones(pawn.addy(), lp.addy(), bounds, matrix, team, is_defusing);
                }
                if (cfg->settings.ESP.Bones.DrawEnemy && !teammate)
                {
                   esp.DrawBones(pawn.addy(), bounds, matrix, false); //isDude
                    // StickFigure(CSPlayerPawn, bounds, matrix, team, is_defusing);
                   // esp.OldDrawBones(pawn.addy(), lp.addy(), bounds, matrix, team, is_defusing);
                }
                confidentlyInGame = true;
                // https://www.unknowncheats.me/forum/general-programming-and-reversing/432465-imgui-drawing.html
                // https://github.com/UnnamedZ03/CS2-external-base/blob/main/source/Source.cpp
            } // end entlist iterate

            PlantedC4 c4 = PlantedC4();
            if(c4.ok() == false)
                goto null;
            
            if (c4.isPlanted() && cfg->settings.ESP.BombTimer)
            {
               
                if(c4.exists()) //|| true
                {
                   
                    float time = c4.TimeRemaining();
                    if ( time >= 0.f)
                    {
                       
                        Vector3 C4origin = c4.origin();
                        C4origin.z += 10.f;
                        Vector3 sC4 = Util::WorldToScreen(bounds, C4origin, matrix);
                        if (sC4.z > 0.01f)
                        {                                                 
                                                
                           
                           
                            if (c4.defusing() && !c4.exploded())
                            {
                                 float defuse_t = c4.defuselength() * -1.f; //magic
                                 char tr[5];
                                 sprintf(tr, "%.1f", defuse_t);
                                 ImVec4 col = ImVec4(0,0.3,0.9,1);
                                 if(c4.defusingwithkit())
                                    col = ImVec4(1,0,1,1);
                                 ImGui::GetOverlayDrawList()->AddText(40.f, ImVec2(sC4.x, sC4.y - 60.f), ImGui::ColorConvertFloat4ToU32(col), tr);
                            }
                            if(!c4.defused() && !c4.exploded()){
                                 char tr[5];
                                 sprintf(tr, "%.1f", time);
                                
                                 ImGui::GetOverlayDrawList()->AddText(40.f, ImVec2(sC4.x, sC4.y), ImGui::ColorConvertFloat4ToU32((time > 10.f) ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1)), tr);
                            }
                          
                           
                        }
                    }
                }
                
            }//end if plantedc4
            else if(!c4.isPlanted() && cfg->settings.ESP.Labels.C4)
            {
               auto weaponc4 =  CC4();
               if(weaponc4.ok() && weaponc4.inPlay())
               {
                    Vector3 og = weaponc4.origin();
                    
                    uintptr_t ownerHandle = weaponc4.hOwnerEntity();
                    Log(ownerHandle, " handle");
                    if(ownerHandle > 0xff && ownerHandle < 0xfffffff )
                    {
                          uintptr_t list_entry2 = *(uintptr_t *)(entity_list + 0x8 * ((ownerHandle & 0x7FFF) >> 9) + 16);
                         if (list_entry2){
                                 uintptr_t c4Owner = *(uintptr_t *)(list_entry2 + 120 * (ownerHandle & 0x1FF));
                                 if(c4Owner)
                                 {
                                     Log(c4Owner, " Owner");
                                 }
                         }
                            
                    }
                    else if(ownerHandle == 0x600ffffffff){
                        Vector3 wC4 = Util::WorldToScreen(bounds, og, matrix);
                        if (wC4.z > 0.01f)
                        {        
                            ImGui::GetOverlayDrawList()->AddText(30.f, ImVec2(wC4.x, wC4.y), ImGui::ColorConvertFloat4ToU32(ImVec4(0.7f,0.2f,0.4f,1.f)), xc("C4"));
                        }               
                    }
                    
               }
                
            } //end carried C4 */



            if (cfg->settings.AIM.drawFOV && cfg->settings.AIM.Enabled)
               ImGui::GetOverlayDrawList()->AddCircle(ImVec2(bounds.x / 2, bounds.y / 2), cfg->settings.AIM.fov / 2, ImGui::ColorConvertFloat4ToU32(ImVec4(0.8, 0.1, 0.1, 0.7)));
        } //end if entlist
    }
    goto null;
null:
}


void KickFlip::SigScan(){

    if(foundOffsets == true)
        return;
    sigs s = sigs();
   LocalPlayerOffset = s.getLocalPlayerOffset();
   EntListOffset = s.getEntityListOffset();
   TraceShapeOffset = s.getTraceShapeOffset();
   EngineTracePtrOffset = s.getEngineTraceOffset();
   ViewMatrixOffset = s.getViewMatrixOffset();
   PlantedC4Offset = s.getPlantedC4Offset();

   if(!LocalPlayerOffset || !EntListOffset || !TraceShapeOffset || !EngineTracePtrOffset || !ViewMatrixOffset || !PlantedC4Offset){
        foundOffsets = false;
        Log(xs("SIG SCAN FAILURE - CRITICAL"));
        return;
   }
    Log(xs("Sig. Scan Success"));
   foundOffsets = true; 
   return;

}