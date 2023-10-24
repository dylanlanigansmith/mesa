#include "config.hpp"
#include "../utils.hpp"


using namespace std;
using namespace json;
Config* cfg = new Config();

Config::Config()
{
    
    loaded = false;
}

Config::~Config()
{
    
}
//https://github.com/nbsdx/SimpleJSON/blob/master/examples/json_example.cpp
string Config::Init()
{
    string home = getenv("HOME");
    if(home.length() > 5){
        path = home + CONFIG_PATH;
    }
    else
        path = "";
    setDefaults();
    loaded = true;
    string err = path;
    if(!FileExists()){
        
        if(saveConfig()){
            string err = "Wrote new config file"; 
            return err + " " + js["VERSION"].ToString();
            return err;
        }
        err = "Couldnt save default cfg";
        return err;
    }
    if (loadConfig())
    {
        loaded = true;
        return err;
    }
    loaded = false;
    return err;
}
bool Config::loadConfig()
{
    string err = "";
    if (path.empty())
        return false;
    ifstream file(path, ifstream::in);
    stringstream ss;
    string j_str;

    if (file.is_open())
    {
        // file >> j_str;
        while (getline(file, j_str))
        {
            ss << j_str;
        }
    }
    file.close();
    try
    {
        js = JSON::Load(ss.str());
    }
    catch (exception &e)
    {
        err = "Json Parse Error";
       
        return false;
    }
    err = "Loaded CFG " + js["VERSION"].ToString();
    loadConfigFromJSON(settings);
    return true;
}
bool Config::load(){
    bool ret = loadConfig();
    if(ret)
        loadConfigFromJSON(settings);
    return ret;
}
void Config::setDefaults()
{
    js = JSON({
        xs("ESP"),{
            xs("Enabled"), 1,
            xs("Labels"),{
                xs("Name"), 0,
                xs("HP"), 1,
                xs("Weapon"), 0,
                xs("C4"), 0,
                xs("Armor"), 0,
                xs("Color"), Array(1.f,1.f,1.f,1.f),
                xs("AlterColor"),1,
                xs("OnlyLabelEnemy"), 1,
                xs("Position"), 1 //0 above, 1-4 TL clockwise around, 5 below
            },
            xs("Box"),{
                xs("DrawEnemy"),1,
                xs("DrawTeam"),1,
                xs("TColor"), Array(114.0f / 255.0f, 200.0f / 255.0f, 220.0f / 255.0f, 1),
                xs("CTColor"), Array(114.0f / 255.0f, 200.0f / 255.0f, 220.0f / 255.0f, 1),
                xs("DColor"), Array(1, 100.f / 255.0f, 0, 1),
                xs("DrawDistance"),0,
                xs("DrawVisible"),0,
                xs("Thickness"),1.f,
                xs("Corners"),0
            },
            xs("Bones"),{
                xs("DrawEnemy"),1,
                xs("DrawTeam"),1,
                xs("TColor"), Array(114.0f / 255.0f, 200.0f / 255.0f, 220.0f / 255.0f, 1),
                xs("CTColor"), Array(114.0f / 255.0f, 200.0f / 255.0f, 220.0f / 255.0f, 1),
                xs("DColor"), Array(1, 100.f / 255.0f, 0, 1),
                xs("DrawDistance"),0,
                xs("DrawVisible"),0,
                xs("Thickness"),1.f,
                xs("Cocks"), 0
            },
            xs("BombTimer"),1,
            xs("FPS Limit"), 100
        },
        xs("KF"),{
            xs("Debug"),1,
            xs("MenuKey"), KEY_BACKSLASH,
            xs("FPS"), 1,
            xs("NoHook"), 1,
            xs("LaunchMenu"), 1
        },
        xs("AIM"),{
            xs("Enabled"),0
        },
        xs("VERSION"),"2.1.0",
    });    

    loadConfigFromJSON(settings);
    loaded = true;
}
bool Config::saveConfig()
{
    if(!isLoaded() || path.length() < 6)
        return false;
    ofstream save;
    try
    {
        save.open(path, ofstream::out);
        save << js << flush;
        save.close();
    }
    catch (std::ios_base::failure &e)
    {
       // Util::ConColorMsg("caught: ");
        //Util::ConColorMsg(e.code().message().c_str());
    }
   // Util::ConColorMsg("didnt catch?");
   
  
   // if (!save.is_open())
       // if (!save.is_open())
        // return false;
   
    return true;
}
bool Config::isLoaded(){
    return loaded;
}
bool Config::FileExists(){
    return filesystem::exists(path);
}
template <typename T>
T Config::get(const std::string &key)
{
}
template <typename T>
void Config::set(const std::string &key, T val)
{
}
ImVec4 Config::ArrayToImVec4( JSON& jsa) {
    if(!js.IsArray())
        return ImVec4(1,1,1,1);
   
   // lastError = jsa.ToString();
    return ImVec4{jsa[0].ToFloat(), jsa[1].ToFloat(), jsa[2].ToFloat(), jsa[3].ToFloat()};
}
ImVec4 Config::ArrayToImVec4( std::string a, std::string b, std::string c ) {
    if(!js[a][b][c].IsArray())
        return ImVec4(1,1,1,1);
    if(js[a][b][c].size() != 4)
        return ImVec4(1,1,1,1);
    lastError = js[a][b][c].ToString();
     
    return ImVec4{js[a][b][c][0].ToFloat(), js[a][b][c][1].ToFloat(), js[a][b][c][2].ToFloat(), js[a][b][c][3].ToFloat()};
}
void Config::loadConfigFromJSON(Config_t& config) {
   // using config as settings; 
    config.ESP.Enabled = js[xs("ESP")][xs("Enabled")].ToBool();
    config.ESP.Labels.Name = js[ xs("ESP")][ xs("Labels") ][ xs("Name")] .ToBool();
    config.ESP.Labels.HP = js[xs("ESP")][xs("Labels")][xs("HP")].ToBool();

    config.ESP.Labels.Weapon = js[xs("ESP")][xs("Labels")][xs("Weapon")].ToBool();
    config.ESP.Labels.C4 = js[xs("ESP")][xs("Labels")][xs("C4")].ToBool();
    config.ESP.Labels.Armor = js[xs("ESP")][xs("Labels")][xs("Armor")].ToBool();
    config.ESP.Labels.Color = ArrayToImVec4(xs("ESP"), xs("Labels"), xs("Color"));
    config.ESP.Labels.AlterColor = js[xs("ESP")][xs("Labels")][xs("AlterColor")].ToBool();
    config.ESP.Labels.OnlyLabelEnemy = js[xs("ESP")][xs("Labels")][xs("OnlyLabelEnemy")].ToBool();
    config.ESP.Labels.Position = js[xs("ESP")][xs("Labels")][xs("Position")].ToInt();

    config.ESP.Box.DrawEnemy = js[xs("ESP")][xs("Box")][xs("DrawEnemy")].ToBool();
    config.ESP.Box.DrawTeam = js[xs("ESP")][xs("Box")][xs("DrawTeam")].ToBool();

    config.ESP.Box.TColor = ArrayToImVec4(xs("ESP"), xs("Box"), xs("TColor"));
    config.ESP.Box.CTColor = ArrayToImVec4(xs("ESP"), xs("Box"), xs("CTColor"));
    config.ESP.Box.DColor = ArrayToImVec4(xs("ESP"), xs("Box"), xs("DColor"));

    config.ESP.Box.DrawDistance = js[xs("ESP")][xs("Box")][xs("DrawDistance")].ToBool();
    config.ESP.Box.DrawVisible = js[xs("ESP")][xs("Box")][xs("DrawVisible")].ToBool();
    config.ESP.Box.Thickness = js[xs("ESP")][xs("Box")][xs("Thickness")].ToFloat();
    config.ESP.Box.Corners = js[xs("ESP")][xs("Box")][xs("Corners")].ToBool();

    config.ESP.Bones.DrawEnemy = js[xs("ESP")][xs("Bones")][xs("DrawEnemy")].ToBool();
    config.ESP.Bones.DrawTeam = js[xs("ESP")][xs("Bones")][xs("DrawTeam")].ToBool();

    config.ESP.Bones.TColor = ArrayToImVec4(xs("ESP"), xs("Bones"), xs("TColor"));
    config.ESP.Bones.CTColor = ArrayToImVec4(xs("ESP"), xs("Bones"), xs("CTColor"));
    config.ESP.Bones.DColor = ArrayToImVec4(xs("ESP"), xs("Bones"), xs("DColor"));

    config.ESP.Bones.DrawDistance = js[xs("ESP")][xs("Bones")][xs("DrawDistance")].ToBool();
    config.ESP.Bones.DrawVisible = js[xs("ESP")][xs("Bones")][xs("DrawVisible")].ToBool();
    config.ESP.Bones.Thickness = js[xs("ESP")][xs("Bones")][xs("Thickness")].ToFloat();
    config.ESP.Bones.Cocks = js[xs("ESP")][xs("Bones")][xs("Cocks")].ToBool();
    config.ESP.Bones.DebugDistance = 1000.f;

    config.ESP.BombTimer = js[xs("ESP")][xs("BombTimer")].ToBool();
    config.ESP.FPSLimit = js[xs("ESP")][xs("FPS Limit")].ToInt();

    config.KF.Debug = js[xs("KF")][xs("Debug")].ToBool();
    config.KF.MenuKey = js[xs("KF")][xs("MenuKey")].ToInt();
    config.KF.FPS = js[xs("KF")][xs("FPS")].ToBool();
    config.KF.NoHook = js[xs("KF")][xs("NoHook")].ToBool();
    config.KF.LaunchMenu = js[xs("KF")][xs("LaunchMenu")].ToBool();
    config.KF.max_logs = 200;

    config.AIM.Enabled = js[xs("AIM")][xs("Enabled")].ToBool();
    config.AIM.punchX = 74.f;
    config.AIM.punchY = 88.1f;
    config.AIM.smoothing = 69;
    config.AIM.delay = 117;
    config.AIM.fov = 120; // / by 2
    config.AIM.max_shift = 5; // % of window width
    config.AIM.max_dist = 450;
    config.AIM.randombone = false;
    config.AIM.randombonechance = 50;
    config.AIM.single_target = true;
    config.AIM.mouse4 = false;
    config.AIM.sens = 2.42f;
    config.AIM.Weapons.Pistols = true;
    config.AIM.Weapons.Rifles = true;
    config.AIM.Weapons.Shotguns = false;
    config.AIM.Weapons.Scoped = false;
    config.AIM.body = false;
    config.AIM.log = false;
    config.AIM.jumping = false;
    config.AIM.drawFOV = false;
    config.VERSION = js[xs("VERSION")].ToString();

    config.Skins.knife = KnifeGUI::karam;
}
void Config::createJSONFromConfig(const Config_t& config) 
{
   
    js[xs("ESP")][xs("Enabled")] = config.ESP.Enabled;
    js[xs("ESP")][xs("Labels")][xs("Name")] = config.ESP.Labels.Name;
    js[xs("ESP")][xs("Labels")][xs("HP")] = config.ESP.Labels.HP;
    js[xs("ESP")][xs("Labels")][xs("Weapon")] = config.ESP.Labels.Weapon;
    js[xs("ESP")][xs("Labels")][xs("C4")] = config.ESP.Labels.C4;
    js[xs("ESP")][xs("Labels")][xs("Armor")] = config.ESP.Labels.Armor;
    js[xs("ESP")][xs("Labels")][xs("Color")] = Array(config.ESP.Labels.Color.x, config.ESP.Labels.Color.y, config.ESP.Labels.Color.z, config.ESP.Labels.Color.w);
    js[xs("ESP")][xs("Labels")][xs("AlterColor")] = config.ESP.Labels.AlterColor;
    js[xs("ESP")][xs("Labels")][xs("OnlyLabelEnemy")] = config.ESP.Labels.OnlyLabelEnemy;
    js[xs("ESP")][xs("Labels")][xs("Position")] = config.ESP.Labels.Position;

    js[xs("ESP")][xs("Box")][xs("DrawEnemy")] = config.ESP.Box.DrawEnemy;
    js[xs("ESP")][xs("Box")][xs("DrawTeam")] = config.ESP.Box.DrawTeam;
    js[xs("ESP")][xs("Box")][xs("TColor")] = Array(config.ESP.Box.TColor.x, config.ESP.Box.TColor.y, config.ESP.Box.TColor.z, config.ESP.Box.TColor.w);
    js[xs("ESP")][xs("Box")][xs("CTColor")] = Array(config.ESP.Box.CTColor.x, config.ESP.Box.CTColor.y, config.ESP.Box.CTColor.z, config.ESP.Box.CTColor.w);
    js[xs("ESP")][xs("Box")][xs("DColor")] = Array(config.ESP.Box.DColor.x, config.ESP.Box.DColor.y, config.ESP.Box.DColor.z, config.ESP.Box.DColor.w);
    js[xs("ESP")][xs("Box")][xs("DrawDistance")] = config.ESP.Box.DrawDistance;
    js[xs("ESP")][xs("Box")][xs("DrawVisible")] = config.ESP.Box.DrawVisible;
    js[xs("ESP")][xs("Box")][xs("Thickness")] = config.ESP.Box.Thickness;
    js[xs("ESP")][xs("Box")][xs("Corners")] = config.ESP.Box.Corners;

    js[xs("ESP")][xs("Bones")][xs("DrawEnemy")] = config.ESP.Bones.DrawEnemy;
    js[xs("ESP")][xs("Bones")][xs("DrawTeam")] = config.ESP.Bones.DrawTeam;
    js[xs("ESP")][xs("Bones")][xs("TColor")] = Array(config.ESP.Bones.TColor.x, config.ESP.Bones.TColor.y, config.ESP.Bones.TColor.z, config.ESP.Bones.TColor.w);
    js[xs("ESP")][xs("Bones")][xs("CTColor")] = Array(config.ESP.Bones.CTColor.x, config.ESP.Bones.CTColor.y, config.ESP.Bones.CTColor.z, config.ESP.Bones.CTColor.w);
    js[xs("ESP")][xs("Bones")][xs("DColor")] = Array(config.ESP.Bones.DColor.x, config.ESP.Bones.DColor.y, config.ESP.Bones.DColor.z, config.ESP.Bones.DColor.w);
    js[xs("ESP")][xs("Bones")][xs("DrawDistance")] = config.ESP.Bones.DrawDistance;
    js[xs("ESP")][xs("Bones")][xs("DrawVisible")] = config.ESP.Bones.DrawVisible;
    js[xs("ESP")][xs("Bones")][xs("Thickness")] = config.ESP.Bones.Thickness;
    js[xs("ESP")][xs("Bones")][xs("Cocks")] = config.ESP.Bones.Cocks;
    js[xs("ESP")][xs("BombTimer")] = config.ESP.BombTimer;
    js[xs("ESP")][xs("FPS Limit")] = config.ESP.FPSLimit;

    js[xs("KF")][xs("Debug")] = config.KF.Debug;
    js[xs("KF")][xs("MenuKey")] = config.KF.MenuKey;
    js[xs("KF")][xs("FPS")] = config.KF.FPS;
    js[xs("KF")][xs("NoHook")] = config.KF.NoHook;
    js[xs("KF")][xs("LaunchMenu")] = config.KF.LaunchMenu;

    js[xs("AIM")][xs("Enabled")] = config.AIM.Enabled;
    
    js[xs("VERSION")] = config.VERSION;
 
}

bool Config::save(){

    createJSONFromConfig(settings);
    return saveConfig();
}