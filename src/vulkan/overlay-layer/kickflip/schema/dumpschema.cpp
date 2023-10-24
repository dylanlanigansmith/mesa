

#include "schemamgr.hpp"
#include "schemasystem.hpp"
#include "../frame.hpp"

 //DevTools::DumpToFile("/home/dylan/Desktop/cs2clientft13ALL.txt");
#ifdef DEVTOOLS
#define HEX(n) std::hex << n << std::dec
using namespace std;
namespace DevTools
{

    const char *client_classes[] = {
        "ActiveModelConfig_t",
        "CAnimGraphNetworkedVariables",
        "CAttributeList",
        "CAttributeManager",
        "CAttributeManager_cached_attribute_float_t",
        "CBaseAnimGraph",
        "CBaseAnimGraphController",
        "CBasePlayerController",
        "CBasePlayerVData",
        "CBasePlayerWeaponVData",
        "CBaseProp",
        "CBodyComponent",
        "CBodyComponentBaseAnimGraph",
        "CBodyComponentBaseModelEntity",
        "CBodyComponentPoint",
        "CBodyComponentSkeletonInstance",
        "CBombTarget",
        "CBuoyancyHelper",
        "CCSGameModeRules",
        "CCSGameModeRules_Deathmatch",
        "CCSObserver_ObserverServices",
        "CCSPlayerBase_CameraServices",
        "CCSPlayerController",
        "CCSPlayerController_ActionTrackingServices",
        "CCSPlayerController_DamageServices",
        "CCSPlayerController_InGameMoneyServices",
        "CCSPlayerController_InventoryServices",
        "CCSPlayer_ActionTrackingServices",
        "CCSPlayer_BulletServices",
        "CCSPlayer_BuyServices",
        "CCSPlayer_CameraServices",
        "CCSPlayer_HostageServices",
        "CCSPlayer_ItemServices",
        "CCSPlayer_MovementServices",
        "CCSPlayer_PingServices",
        "CCSPlayer_ViewModelServices",
        "CCSPlayer_WaterServices",
        "CCSPlayer_WeaponServices",
        "CCSWeaponBaseVData",
        "CClientAlphaProperty",
        "CCollisionProperty",
        "CComicBook",
        "CCompositeMaterialEditorDoc",
        "CDamageRecord",
        "CDecalInfo",
        "CEconItemAttribute",
        "CEffectData",
        "CEntityIdentity",
        "CEntityInstance",
        "CFireOverlay",
        "CFlashlightEffect",
        "CFuncWater",
        "CGameSceneNode",
        "CGameSceneNodeHandle",
        "CGlobalLightBase",
        "CGlowOverlay",
        "CGlowProperty",
        "CGlowSprite",
        "CGrenadeTracer",
        "CHitboxComponent",
        "CInfoDynamicShadowHint",
        "CInfoDynamicShadowHintBox",
        "CInfoOffscreenPanoramaTexture",
        "CInfoWorldLayer",
        "CInterpolatedValue",
        "CLightComponent",
        "CLogicRelay",
        "CModelState",
        "CNetworkedSequenceOperation",
        "CPlayer_CameraServices",
        "CPlayer_MovementServices",
        "CPlayer_MovementServices_Humanoid",
        "CPlayer_ObserverServices",
        "CPlayer_WeaponServices",
        "CPointOffScreenIndicatorUi",
        "CPointTemplate",
        "CPrecipitationVData",
        "CProjectedTextureBase",
        "CRenderComponent",
        "CSMatchStats_t",
        "CSPerRoundStats_t",
        "CScriptComponent",
        "CSkeletonInstance",
        "CSkyboxReference",
        "CTimeline",
        "C_AttributeContainer",
        "C_BarnLight",
        "C_BaseButton",
        "C_BaseCSGrenade",
        "C_BaseCSGrenadeProjectile",
        "C_BaseClientUIEntity",
        "C_BaseCombatCharacter",
        "C_BaseDoor",
        "C_BaseEntity",
        "C_BaseFire",
        "C_BaseFlex",
        "C_BaseFlex_Emphasized_Phoneme",
        "C_BaseGrenade",
        "C_BaseModelEntity",
        "C_BasePlayerPawn",
        "C_BasePlayerWeapon",
        "C_BasePropDoor",
        "C_BaseTrigger",
        "C_BaseViewModel",
        "C_Beam",
        "C_BreakableProp",
        "C_BulletHitModel",
        "C_C4",
        "C_CSGOViewModel",
        "C_CSGO_MapPreviewCameraPath",
        "C_CSGO_MapPreviewCameraPathNode",
        "C_CSGO_PreviewModel",
        "C_CSGO_PreviewPlayer",
        "C_CSGO_TeamPreviewCamera",
        "C_CSGO_TeamPreviewCharacterPosition",
        "C_CSGameRules",
        "C_CSGameRulesProxy",
        "C_CSObserverPawn",
        "C_CSPlayerPawn",
        "C_CSPlayerPawnBase",
        "C_CSPlayerResource",
        "C_CSTeam",
        "C_CSWeaponBase",
        "C_CSWeaponBaseGun",
        "C_Chicken",
        "C_ClientRagdoll",
        "C_ColorCorrection",
        "C_ColorCorrectionVolume",
        "C_CommandContext",
        "C_CsmFovOverride",
        "C_DecoyProjectile",
        "C_DynamicLight",
        "C_DynamicProp",
        "C_EconEntity",
        "C_EconEntity_AttachedModelData_t",
        "C_EconItemView",
        "C_EconWearable",
        "C_EntityDissolve",
        "C_EntityFlame",
        "C_EnvCombinedLightProbeVolume",
        "C_EnvCubemap",
        "C_EnvCubemapFog",
        "C_EnvDecal",
        "C_EnvDetailController",
        "C_EnvLightProbeVolume",
        "C_EnvParticleGlow",
        "C_EnvScreenOverlay",
        "C_EnvSky",
        "C_EnvVolumetricFogController",
        "C_EnvVolumetricFogVolume",
        "C_EnvWind",
        "C_EnvWindClientside",
        "C_EnvWindShared",
        "C_EnvWindShared_WindAveEvent_t",
        "C_EnvWindShared_WindVariationEvent_t",
        "C_FireSmoke",
        "C_FireSprite",
        "C_Fish",
        "C_Fists",
        "C_FogController",
        "C_FootstepControl",
        "C_FuncConveyor",
        "C_FuncElectrifiedVolume",
        "C_FuncLadder",
        "C_FuncMonitor",
        "C_FuncTrackTrain",
        "C_GlobalLight",
        "C_GradientFog",
        "C_HandleTest",
        "C_Hostage",
        "C_Inferno",
        "C_InfoVisibilityBox",
        "C_IronSightController",
        "C_Item",
        "C_ItemDogtags",
        "C_LightEntity",
        "C_LightGlow",
        "C_LightGlowOverlay",
        "C_LocalTempEntity",
        "C_MapVetoPickController",
        "C_Melee",
        "C_MolotovProjectile",
        "C_Multimeter",
        "C_OmniLight",
        "C_ParticleSystem",
        "C_PathParticleRope",
        "C_PhysMagnet",
        "C_PhysPropClientside",
        "C_PhysicsProp",
        "C_PickUpModelSlerper",
        "C_PlantedC4",
        "C_PlayerPing",
        "C_PlayerSprayDecal",
        "C_PlayerVisibility",
        "C_PointCamera",
        "C_PointCameraVFOV",
        "C_PointClientUIDialog",
        "C_PointClientUIHUD",
        "C_PointClientUIWorldPanel",
        "C_PointClientUIWorldTextPanel",
        "C_PointCommentaryNode",
        "C_PointValueRemapper",
        "C_PointWorldText",
        "C_PostProcessingVolume",
        "C_Precipitation",
        "C_PredictedViewModel",
        "C_RagdollManager",
        "C_RagdollProp",
        "C_RagdollPropAttached",
        "C_RectLight",
        "C_RetakeGameRules",
        "C_RopeKeyframe",
        "C_RopeKeyframe_CPhysicsDelegate",
        "C_SceneEntity",
        "C_SceneEntity_QueuedEvents_t",
        "C_ShatterGlassShardPhysics",
        "C_SkyCamera",
        "C_SmokeGrenadeProjectile",
        "C_SoundAreaEntityBase",
        "C_SoundAreaEntityOrientedBox",
        "C_SoundAreaEntitySphere",
        "C_SoundOpvarSetPointBase",
        "C_SpotlightEnd",
        "C_Sprite",
        "C_Sun",
        "C_SunGlowOverlay",
        "C_Team",
        "C_TeamRoundTimer",
        "C_TextureBasedAnimatable",
        "C_TonemapController2",
        "C_TriggerBuoyancy",
        "C_ViewmodelWeapon",
        "C_VoteController",
        "C_WeaponBaseItem",
        "C_WeaponShield",
        "C_WeaponTaser",
        "C_fogplayerparams_t",
        "CompMatMutatorCondition_t",
        "CompMatPropertyMutator_t",
        "CompositeMaterialAssemblyProcedure_t",
        "CompositeMaterialEditorPoint_t",
        "CompositeMaterialInputContainer_t",
        "CompositeMaterialInputLooseVariable_t",
        "CompositeMaterialMatchFilter_t",
        "CompositeMaterial_t",
        "CountdownTimer",
        "EngineCountdownTimer",
        "EntityRenderAttribute_t",
        "EntitySpottedState_t",
        "GeneratedTextureHandle_t",
        "IntervalTimer",
        "PhysicsRagdollPose_t",
        "SellbackPurchaseEntry_t",
        "ServerAuthoritativeWeaponSlot_t",
        "TimedEvent",
        "VPhysicsCollisionAttribute_t",
        "ViewAngleServerChange_t",
        "WeaponPurchaseCount_t",
        "WeaponPurchaseTracker_t",
        "audioparams_t",
        "fogparams_t",
        "shard_model_desc_t",
        "sky3dparams_t"

    };
    const char *test_classes[] = {
        "C_CSPlayerPawn",
        "C_CSPlayerPawnBase",
        "C_CSPlayerResource",
        "C_CSTeam",
        "C_CSWeaponBase",
        "C_CSWeaponBaseGun",
        "CCSPlayerController",
        "C_BaseEntity",
        "C_PlantedC4",
    };
    bool DumpToFile(string path)
    {
        int sizec = 263; // vcscode highlight lol

        ofstream save;
        // char myBuffer[8192];
        //  save.rdbuf()->pubsetbuf(myBuffer, 8192);
        std::filebuf fb;
        try
        {
            if (!fb.open(path, std::ios::out))
            {
                kf->Log("No open");
                return false;
            }
            std::ostream save(&fb);
            if(save.fail()){
                kf->Log("Fail at Open");
                kf->Log(std::to_string(save.badbit));
                return false;
            }
            kf->Log("Opened file @" + path);

            int successful = 0;
            for (int i = 0; i < sizec; i++)
            {
                save << client_classes[i] << "\n"
                     << "{"
                     << endl;
                CSchemaSystemTypeScope *typeScope = CSchemaSystem::Get()->FindTypeScopeForModule(CLIENT_LIBX);
                if (!typeScope)
                {
                    save << "\t"
                         << "Typescope Not Found" << endl;
                    continue;
                }
                CSchemaClassInfo *classInfo = typeScope->FindDeclaredClass(client_classes[i]);
                if (!classInfo)
                {
                    save << "    " 
                         << "Class Not Found" << endl;
                    continue;
                }
                kf->Log("Fields count for class " + std::string(client_classes[i]) + ": " + std::to_string(classInfo->m_nFieldsCount));
                if (classInfo->m_nFieldsCount == 0)
                {
                    save << "    " 
                         << "Class Not Found" << endl;
                    continue;
                }
                if (!classInfo->m_Fields)
                {

                
                    save << "    " 
                         << "Fields Not Found" << endl;
                    continue;
                }
                for (int j = 0; j < classInfo->m_nFieldsCount; ++j)
                {
                    auto &field = classInfo->m_Fields[j];
                    int offset = -1;
                    if (!field.m_Name)
                        continue;
                    if (field.m_nSingleInheritanceOffset >= 0)
                        offset = field.m_nSingleInheritanceOffset;
                    std::string name = field.m_Name;

                    char buf[32];
                    sprintf(buf, "0x%x", offset);
                    string hexed = buf;
                    save << "    " << name << " = " << hexed << endl; // HEX(offset)
                  //  std::this_thread::sleep_for(std::chrono::microseconds(100));
                    if(save.fail()){
                        kf->Log("Fail at " + name);
                        kf->Log(std::to_string(save.badbit));
                    }
                        
                }
                kf->Log("Wrote " + std::string(client_classes[i]));
                save << "}" << endl;

                successful++;
            }

            save << "Wrote info for " << successful << " / " << sizec << " classes." << endl;

            save.flush();

            kf->Log("Closed");
            return true;
        }
        catch (std::ios_base::failure &e)
        {
            kf->Log("File Failure" + std::string(e.what()));
            return false;
        }
    }

} // end DevTool

#endif