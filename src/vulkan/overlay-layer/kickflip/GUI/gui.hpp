#pragma once
#include "../includes.hpp"
#include "../cfg/config.hpp"

class CGUI
{
public:
    CGUI()
    {
        hasStyle = false; //if only i had pants like ross
        appliedCfg = false;
        currentTab = 0;
        colorSelect = 0;
        sensSelect = 0; showSettings = true;
        box_color_defuse = ImVec4(1, 100.f / 255.0f, 0, 1);
        box_color_ct = ImVec4(114.0f / 255.0f, 200.0f / 255.0f, 220.0f / 255.0f, 1); // 155
        box_color_t = ImVec4(242.0f / 255.0f, 117.0f / 255.0f, 210.f / 255.0f, 1);
        esp_text_color = ImVec4(1.f,1.f,1.f,1.f);
        //enabled = cfg->js["ESP"]["Enabled"].ToBool();
    }
    ~CGUI() {}
    void setStyle();
    void drawTabBar(const char *name, int idx);
    void drawESPTab();
    void drawMainTab();
    void drawDevTab();
    void drawAimTab();
    void update();
    int colorSelect;
    int currentTab;

    std::vector<std::string> debug;

    //ESP
    bool ESPenabled;
    bool ESPC4enabled;

    ImVec4 box_color_defuse;
    ImVec4 box_color_ct;
    ImVec4 esp_text_color;
    ImVec4 box_color_t;
    bool hasStyle;
    bool appliedCfg;

    //AIM
    int sensSelect;
    bool showSettings;
};