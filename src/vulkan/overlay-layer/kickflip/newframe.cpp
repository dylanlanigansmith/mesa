#include "includes.hpp"
#include "frame.hpp"
#include "utils.hpp"
#include "offsets.hpp"
#include "cfg/config.hpp"
#include "sdk/sdk.hpp"
#include "sdk/getvar.hpp"
#include "sdk/classes/localplayer.hpp"
void KickFlip::NewFrame(ImGuiContext *imgui_context, float fps, ImVec2 win_size, ImVec2 win_dim)
{
//traps: VKRenderThread[2124] general protection fault ip:7f044003dbb3 sp:7f03a2bfd1c0 error:0 in libflipover.so (deleted)[7f0440026000+a0000]
   // ImGui::SetCurrentContext(imgui_context);
  
   
   //ADD A CHECK IF LIB WASNT DELETED AND CRASH GAME IF IT WASNT 
   
    if(!cfg->isLoaded())
   {
        Log(cfg->Init(), (uint8_t)LOG_INFO);
        menuOpen = cfg->settings.KF.LaunchMenu; //badddd
       // version = cfg->settings.VERSION;
        //assert(cfg->isLoaded());
    } 
    if (GetInput()->hasInitialized())
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MousePos = GetInput()->MousePos();
        io.MouseDown[0] = GetInput()->Mouse(1);
        io.MouseDown[1] = GetInput()->Mouse(2);
        if (GetInput()->ShouldOpenMenu())
        {
            menuOpen = true;
        }
    }
   
    if(!bounds.x){
        bounds.x = win_dim.x; // draw_data->DisplaySize.x
        bounds.y = win_dim.y; //if w2s broke this is why
        aim->setBounds(bounds);
    }
    ImGuiWindowFlags flags = (menuOpen) ? 0 : ImGuiWindowFlags_NoMouseInputs;
    bool overlay = true;
   
   



  //  if(cfg->js["KF"]["FPS"].ToBool())
    char buf[128];
    sprintf(buf, "Kickflip %s [%s] %.0f###KFMain",version.c_str(),(status ? "ok" : "bad"), fps);
     const float margin = 10.0f;
    if(!menuOpen){
        ImGui::SetNextWindowSize(win_size, ImGuiCond_Always); //ahhh
         ImGui::SetNextWindowPos(ImVec2(win_dim.x - win_size.x - margin, margin + 10),
                            ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.5);
        ImGui::SetNextWindowCollapsed(true);
    }
    else {
        ImGui::SetNextWindowSize(ImVec2(win_size.x * 4, win_size.y * 3), ImGuiCond_Always);
         ImGui::SetNextWindowPos(ImVec2(win_dim.x - (win_size.x * 4) - margin, margin + 10),
                            ImGuiCond_Always); //AHAHHAHAHH
        ImGui::SetNextWindowBgAlpha(0.8);
        ImGui::SetNextWindowCollapsed(false);
    }
    //if(!gui.hasStyle)
      //  gui.setStyle(); //? why no persist FUGLY
    ImGui::Begin(buf, &overlay, flags);
    if (menuOpen)
    {
         gui.drawTabBar(xc("Home"),0);
         gui.drawTabBar(xc("ESP"),1);
         gui.drawTabBar(xc("Dev"),2);
         gui.drawTabBar(xc("Aim"),3);
         gui.drawTabBar(xc("Save"), 4);
        gui.drawTabBar(xc("Close Menu"), 5);
         switch(gui.currentTab){
            case 0:
                gui.drawMainTab();break;
            case 1:
                gui.drawESPTab(); break;
            case 2: 
                gui.drawDevTab(); break;
            case 3:
                gui.drawAimTab(); break;
            case 4:
                cfg->save(); gui.currentTab = 0;
                break;
            case 5:
                menuOpen = false; gui.currentTab = 0; break;
            default:
                gui.drawMainTab();
         }

    
     
        if(ImGui::Button("crash game")){
           
            Log(kf->off->get(xs("CSkeletonInstance"), xs("m_modelState")), "h"); //uint32t
           
            Log(kf->off->get(xs("CCSPlayerController"),xs("m_iPendingTeamNum")), "cw"); //flt might need m_hPawn
            
            Log(kf->off->get(xs("C_BaseModelEntity"), xs("m_vecViewOffset")), "gsn");
            Log(kf->off->get(xs("CGameSceneNode"), xs("m_vecOrigin")), "vo");
        }
       
        if (ImGui::Button("close"))
        {
            menuOpen = false;
        }
    }
   
    ImGui::End();


    uintptr_t client_start = 0;
   



   
    if(ok && GetMem()->Ready()){
         auto& cl = GetMem()->GetModule(CLIENT_LIBX);
         if(cl)
             client_start = cl->start;
    }
   

    status = (client_start > 0);
    

    bool confidentlyInGame = false; //TIME TO LOOK AT ENGINE 
    int minfps = 100;
    if(cfg->settings.ESP.FPSLimit > 80)
        minfps = cfg->settings.ESP.FPSLimit;
    if (client_start != 0 && fps > minfps)
    { // bad
        auto& cl = GetMem()->GetModule(CLIENT_LIBX);
        if(!cl)
            goto null;
        LocalPlayer lp = LocalPlayer(cl);
       
        
        if (!lp.validate() || !CEngineClient::Get()->IsInGame())
        {

            // ImGui::Text("loading...");
            goto null;
        }
        uintptr_t entity_list = *(uintptr_t *)(client_start + ENT_OFFSET);
        if (entity_list)
        {
            
            view_matrix_t matrix = *(view_matrix_t *)(client_start + VIEW_MATRIX);
            
            
            
            bool isSomeoneDefusing = false;
            for (int i = 1; i < ENT_MAX; i++)
            {
                
                
                if (!lp.validate())
                    continue;
                PlayerController player = PlayerController(PlayerController::GetPlayerFromIndex(entity_list, i));
                if(!player.validate())
                    continue;
                
                if(player.addy() == lp.addy())
                    continue;
               
               
                if(!player.isAlive())
                    continue;
                
                
                int team = player.team();
               
                if (team < Teams::ALL)
                    continue;
                if (team > Teams::CT)
                    continue;

                bool label = true;
                bool teammate = false;
                if(team == lp.team() ){
                    teammate = true;
                    if(!lp.isAlive())
                        continue;
                    else if(cfg->settings.ESP.Labels.OnlyLabelEnemy){
                        label = false;
                        
                    }
                }
             
               
                std::string name;
                
                if(cfg->settings.ESP.Labels.Name && label)
                  name = player.name();

                if(false){
                    if (name.find(xs("dude")) != std::string::npos && name.find("car") != std::string::npos)
                        Log(xs("kf2024"));
                }
                
                CSPlayerPawn pawn = CSPlayerPawn(player.pawn(entity_list));
                if(!pawn.validate())
                    continue;
                


                int health = pawn.health();
                  
                if (!(health >= 0) || (health > 100) || health == 0) // bad
                    continue;

                
                if(!pawn.movedSinceSpawn())
                    label = false;
                std::string WeaponName = "None";
                if (cfg->settings.ESP.Labels.Weapon && label) {
                    CWeapon weapon = pawn.clippingWeapon();
                    if(weapon.validate()){
                        weapon.itemDefinitionIndex();
                        WeaponName = weapon.name();
                    }
                        
                }

                
                uint8_t is_defusing = 0;
                if (team == 3 )
                {
                    is_defusing = pawn.defusing(); //if always true its broken
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

            
               
                // ImGui::GetOverlayDrawList()->AddCircle(ImVec2(screen.x, screen.y), screen.z, ImGui::ColorConvertFloat4ToU32(ImVec4(1, 0, 0, 1)), 8);
                
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
                
                if(cfg->settings.ESP.Labels.HP && label){
                    std::string utf_8_1 = std::to_string(health); // fuck utf
                    float hp_mod = (health * 0.01);
                    ImVec4 clr = (cfg->settings.ESP.Labels.AlterColor) ? ImVec4(1, hp_mod, hp_mod, 1) : cfg->settings.ESP.Labels.Color;
                    ImGui::GetOverlayDrawList()->AddText(ImVec2(x + w, y), ImGui::ColorConvertFloat4ToU32(clr), utf_8_1.c_str());
                }
                    
                if(cfg->settings.ESP.Labels.Name && label){
                    
                   
                    ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y + 12.f), ImGui::ColorConvertFloat4ToU32(cfg->settings.ESP.Labels.Color), name.c_str());
                }
                if(cfg->settings.ESP.Labels.Weapon && label){
                    ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y + h + 5.f), ImGui::ColorConvertFloat4ToU32(cfg->settings.ESP.Labels.Color), WeaponName.c_str());
                }
                if (cfg->settings.ESP.Bones.DrawTeam && teammate)
                {
                   // StickFigure(CSPlayerPawn, bounds, matrix, team, is_defusing);
                   esp.DrawBones(pawn.addy(), lp.addy(), bounds, matrix, team,  is_defusing);
                }
                if (cfg->settings.ESP.Bones.DrawEnemy && !teammate)
                {
                   // StickFigure(CSPlayerPawn, bounds, matrix, team, is_defusing);
                   esp.DrawBones(pawn.addy(), lp.addy(), bounds, matrix, team,  is_defusing);
                }
                confidentlyInGame = true;
                // https://www.unknowncheats.me/forum/general-programming-and-reversing/432465-imgui-drawing.html
                // https://github.com/UnnamedZ03/CS2-external-base/blob/main/source/Source.cpp
            }
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
            }
        }
    }
    goto null;
null:
    
    
    
}