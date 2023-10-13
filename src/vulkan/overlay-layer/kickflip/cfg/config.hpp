#pragma once
#include "../includes.hpp"
#include "../utils.hpp"
#include "json.hpp"


#define CONFIG_PATH xs("/kickflip.json")

using namespace json;

struct Config_t;


struct Config_t {
    struct ESP {
        bool Enabled;
        struct Labels {
            bool Name;
            bool HP;
            bool Weapon;
            bool C4;
            bool Armor;
            ImVec4 Color;
            bool AlterColor;
            bool OnlyLabelEnemy;
            bool Position;
        } Labels;
        
        struct Box {
            bool DrawEnemy;
            bool DrawTeam;
            ImVec4 TColor;
            ImVec4 CTColor;
            ImVec4 DColor;
            int DrawDistance;
            bool DrawVisible;
            float Thickness;
            int Corners;
        } Box;
        
        struct Bones {
            bool DrawEnemy;
            bool DrawTeam;
            ImVec4 TColor;
            ImVec4 CTColor;
            ImVec4 DColor;
            int DrawDistance;
            bool DrawVisible;
            float Thickness;
            bool Cocks;
        } Bones;
        
        bool BombTimer;
        int FPSLimit;
    } ESP;

    struct KF {
        int Debug;
        int MenuKey;
        int FPS;
        int NoHook;
        int LaunchMenu;
    } KF;
    
    struct AIM {
        int Enabled;
        float punchX;
        float punchY;
        int delay;
        int smoothing;
        int fov;
        int max_shift;
        int max_dist;
        bool randombone;
        int randombonechance;
        bool single_target;
        bool mouse4;
        float sens;
        bool log;
        bool body;
        struct Weapons{
            bool Rifles;
            bool Pistols;
            bool Shotguns;
            bool Scoped;
        } Weapons;
    } AIM;
    
    std::string VERSION;
};
class Config{
    public:
        Config();
        ~Config();
        std::string lastError;
        Config_t settings;
        std::string Init();
        bool loadConfig();
        void setDefaults();
        bool saveConfig();
        bool save();
        bool load();
        bool isLoaded();
        void loadConfigFromJSON(Config_t& config); 
        void createJSONFromConfig(const Config_t& config); 
        template <typename T> T get(const std::string& key);
        template <typename T> void set(const std::string& key, T val);
        std::string path;
       // std::unordered_map<std::string, std::unordered_map<std::string, std::variant<uint8_t, ImVec4, std::string, bool> > > cache; //fuck  
    
        JSON js; //BAD
    private:
         ImVec4 ArrayToImVec4(JSON& jsa);
         ImVec4 ArrayToImVec4( std::string a, std::string b, std::string c );
        bool FileExists();
        bool loaded;
        
};
extern Config* cfg;
//JSON OBJ for each cfg category
/* for each key pair load into unordered_map ???
 * idk lets see how this is
 * 
 * 
 * 
 * 
 * 
 * 
*/