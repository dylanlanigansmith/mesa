#include "gui.hpp"
#include "../cfg/config.hpp"
#include "../frame.hpp"
const char* colorOptions[] = { "---", "T", "CT", "Defusing", "Text" };

const char* sensOptions[] = { "2.42", "0.6", "0.75"};
void CGUI::drawTabBar(const char* name, int idx)
{
    bool ChangedStyle = false;
	ImGui::SameLine(); 
	if (currentTab == idx)
	{ //styles for an active tab button
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(42 / 255.f,82 / 255.f, 80 / 255.f, 1.f)); 
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(70 / 255.f,89 / 255.f, 88 / 255.f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(91 / 255.f,82 / 105.f, 104 / 255.f, 1.f)); //circa 2015 holy
		ChangedStyle = true;
	}
	
	if (ImGui::Button(name))
		currentTab = idx;
	if (ChangedStyle)
		ImGui::PopStyleColor(3); //make sure i get changed if more styles are added
}
void CGUI::drawESPTab(){
   
    
    ImGui::Columns(2, xc("locations"));
    ImGui::Checkbox(xc("Enabled"),  &(cfg->settings.ESP.Enabled));
    ImGui::Checkbox(xc("Bomb Timer"), &(cfg->settings.ESP.BombTimer));
    ImGui::Separator();
    ImGui::Text("%s", xc("Text Display"));
    ImGui::BeginChild("textfilter", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.4f, 19 * 6), true);
    {
        ImGui::Selectable(xc("Name"), &cfg->settings.ESP.Labels.Name);
        ImGui::Selectable(xc("HP"), &cfg->settings.ESP.Labels.HP);
        ImGui::Selectable(xc("Weapon"), &cfg->settings.ESP.Labels.Weapon);
        ImGui::Selectable(xc("Has C4"), &cfg->settings.ESP.Labels.C4);
        ImGui::Selectable(xc("Armor"), &cfg->settings.ESP.Labels.Armor);
    }
    ImGui::EndChild();
    ImGui::Checkbox(xc("Only Label Enemy"),  &(cfg->settings.ESP.Labels.OnlyLabelEnemy));
    
    ImGui::NextColumn();
    ImGuiColorEditFlags cp_flags = 0; cp_flags = 0; //ImGuiColorEditFlags_PickerHueWheel; 
    cp_flags = ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_AlphaBar; //ImGuiColorEditFlags_NoPicker 
   
    ImGui::Combo("Edit Color:", &colorSelect, colorOptions, IM_ARRAYSIZE(colorOptions)); 
    switch (colorSelect){
        case 0:
            break;
        case 1:
             ImGui::ColorPicker4(xc("T Color"), &cfg->settings.ESP.Box.TColor.x, cp_flags); break;
        case 2:
            ImGui::ColorPicker4(xc("CT Color"), &cfg->settings.ESP.Box.CTColor.x, cp_flags); break;
        case 3:
            ImGui::ColorPicker4(xc("Defuse Color"), &cfg->settings.ESP.Box.DColor.x, cp_flags); break;
        case 4:
            ImGui::ColorPicker4(xc("Text Color"), &cfg->settings.ESP.Box.TColor.x, cp_flags); break;
    }
    ImGui::Columns;
    ImGui::Separator();
    ImGui::Text("Boxes");
    ImGui::Checkbox(xc("Draw For Team"), &cfg->settings.ESP.Box.DrawTeam); ImGui::SameLine(); ImGui::Checkbox(xc("Draw For Enemy"), &cfg->settings.ESP.Box.DrawEnemy);
    ImGui::NewLine();
    ImGui::Text("Bones");
    ImGui::Checkbox(xc("Draw For Team###b"), &cfg->settings.ESP.Bones.DrawTeam); ImGui::SameLine(); ImGui::Checkbox(xc("Draw For Enemy###be"), &cfg->settings.ESP.Bones.DrawEnemy);
     ImGui::Checkbox(xc("Cocks (beta)"), &cfg->settings.ESP.Bones.Cocks);
    ImGui::Separator();
    ImGui::SliderInt(xc("Cutoff FPS"), &cfg->settings.ESP.FPSLimit, 80, 250, "%i fps");
}

void CGUI::drawAimTab()
{
    ImGui::Text("%s",xc("Use at own risk"));
    ImGui::Checkbox("Enabled", (bool*)&cfg->settings.AIM.Enabled);
    if(ImGui::Button(xc("kill")) ){
        kf->getAim()->reset();
    }
    std::string state = "no state";
    switch(kf->getAim()->getState())
    {
        case aims::OFF:
            state = "OFF"; break;
         case aims::IDLE:
            state = "IDLE"; break;
        case aims::AT_TARGET:
            state = xs("AT_TARGET"); break;
        case aims::HAS_TARGET:
            state = xs("HAS TARGET"); break;
        case aims::SHUTDOWN:
            state = "SHUTDOWN"; break;   
        case aims::LOCKING:
            state = "LOCKING"; break;   
        default:
            state = "unknown";
    }
    if(ImGui::Button("Settings"))
        showSettings = !showSettings;
    if(!showSettings)
        return;
    ImGui::Separator();
    ImGui::Columns(2, xc("aimlocat"));
    ImGui::Text("state: %s",state.c_str());
    ImGui::Text("bounds: %f %f",kf->getAim()->getBounds().x, kf->getAim()->getBounds().y);
    ImGui::SliderFloat(xc("XRCS"), &cfg->settings.AIM.punchX, 0.f, 100.f,"%.3f");
    ImGui::SliderFloat(xc("YRCS"), &cfg->settings.AIM.punchY, 0.f, 100.f, "%.3f");
    ImGui::SliderInt(xc("Step Time"), &cfg->settings.AIM.delay, 0, 1500, "%i");
    ImGui::SliderInt(xc("Smoothing (no effect)"), &cfg->settings.AIM.smoothing, 0, 100, "%i");
    ImGui::SliderInt(xc("Max Shift"), &cfg->settings.AIM.max_shift, 5, 100, "%i %% of screen");
    ImGui::SliderInt(xc("Max Dist."), &cfg->settings.AIM.max_dist, 5, 4000, "%i ");
    ImGui::SliderInt(xc("FOV"), &cfg->settings.AIM.fov, 5, 600, xc("%i pixels around xhair"));
    ImGui::Checkbox(xc("Swap Mouse Sidebutton"), &cfg->settings.AIM.mouse4);
    ImGui::Checkbox(xc("Single Target"), &cfg->settings.AIM.single_target);
    ImGui::Checkbox(xc("Random Bone"), &cfg->settings.AIM.randombone);
    if(cfg->settings.AIM.randombone)
        ImGui::SliderInt(xc("Chance"), &cfg->settings.AIM.randombonechance, 1, 100, xc("%i %% bias to body"));
    else
        ImGui::Checkbox(xc("Body"), &cfg->settings.AIM.body);
    ImGui::NextColumn();
    ImGui::BeginChild("wfilter", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.4f, 19 * 4), true);
    {
        ImGui::Selectable(xc("Rifles/SMGs"), &cfg->settings.AIM.Weapons.Rifles);
        ImGui::Selectable(xc("Shotguns"), &cfg->settings.AIM.Weapons.Shotguns);
        ImGui::Selectable(xc("Pistols"), &cfg->settings.AIM.Weapons.Pistols);
    }
    ImGui::EndChild();
     ImGui::Combo(xc("Sensitivity"), &sensSelect, sensOptions, IM_ARRAYSIZE(sensOptions)); 
     switch (sensSelect){
        case 0:
            cfg->settings.AIM.sens = 2.42f; break;
        case 1: 
            cfg->settings.AIM.sens = 0.6f; break;
        case 2:
            cfg->settings.AIM.sens = 0.75f; break;
        default:
            cfg->settings.AIM.sens = 0.f;

     }
    ImGui::Text("%s -> %0.3f", xc("Mouse Sensitivity"), kf->getAim()->userSens() );
    ImGui::Text("%s",xc("You have to press lock every time you want it."));
    ImGui::Text("%s",xc("I haven't test weapon filter yet"));
    ImGui::Columns;
}

void CGUI::drawMainTab()
{
    ImGui::Text("Welcome to Kickflip ");
    ImGui::Checkbox(xc("Open Menu On Launch"), (bool*)&cfg->settings.KF.LaunchMenu);
    
}
void CGUI::drawDevTab()
{
    ImGui::Text("Logs: ");
    for(auto& l : kf->logs){
        ImGui::Text(">%s",l.c_str());
    }
    ImGui::Separator();
     for(auto& l : debug){
        ImGui::Text("%s", l.c_str());
    }
    ImGui::Text("arr: %s", cfg->lastError.c_str());
   
    if(ImGui::Button("Clear")){
         debug.clear();
         kf->logs.clear();
    }
   
}
void CGUI::setStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] =			ImVec4(0.1f, 0.1f, 0.1f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] =   ImVec4(0.6f, 0.6f, 0.6f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] =		ImVec4(0.502, 0.502, 0.502, 1.00f);
	style.Colors[ImGuiCol_ChildBg] =		ImVec4(0.402, 0.402, 0.402, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg] =  ImVec4(0.402, 0.402, 0.402, 1.00f);
	style.Colors[ImGuiCol_Border] =			ImVec4(0.61f, 0.61f, 0.61f, 0.80f);
	style.Colors[ImGuiCol_FrameBg] =		ImVec4(0.80f, 0.80f, 0.80f, 0.09f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] =  ImVec4(0.04f, 0.04f, 0.04f, 0.88f);
	style.Colors[ImGuiCol_TitleBg] =		ImVec4(0.686f, 0.243f, 0.169f, 0.90f); //0.686f, 0.243f, 0.169f
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
	style.Colors[ImGuiCol_TitleBgActive] =  ImVec4(0.686f, 0.243f, 0.169f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] =	    ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] =    ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] =      ImVec4(0.31, 0.31, 0.425, 1.0f);
	
	style.Colors[ImGuiCol_ScrollbarGrab] =			ImVec4(0.984f, 0.82f, 0.345f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] =	ImVec4(0.984f, 0.82f, 0.345f, 0.59f); 
	style.Colors[ImGuiCol_ScrollbarGrabActive] =	ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] =				ImVec4(1.00f, 1.00f, 1.00f, 0.59f);
	style.Colors[ImGuiCol_SliderGrabActive] =		ImVec4(0.984f, 0.82f, 0.345f, 1.00f);
	style.Colors[ImGuiCol_Button] =				    ImVec4(0.984f, 0.82f, 0.345f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] =			ImVec4(0.667f, 0.506f, 0.016f, 0.59f);
	style.Colors[ImGuiCol_ButtonActive] =			ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_Header] =					ImVec4(0.984f, 0.82f, 0.345f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] =			ImVec4(0.99f, 0.9f, 0.5f, 0.59f);
	style.Colors[ImGuiCol_HeaderActive] =			ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] =			ImVec4(0.10f, 0.72f, 0.60f, 0.22f);
	//style.Colors[ImGuiCol_] =			ImVec4(0.071f, 0.024f, 0.016f, 1.00f);
	//style.Colors[ImGuiCol_CloseButtonHovered] =		ImVec4(0.071, 0.024, 0.016, 0.59f);

	style.WindowRounding = 0.75f;
	style.FramePadding = ImVec2(8, 1);
	style.ScrollbarSize = 10.f;
	style.ScrollbarRounding = 0.1f;
	style.GrabMinSize = 5.f;
    hasStyle = true;
}
void CGUI::update(){
    //SO BAD BUT SO EASY


}