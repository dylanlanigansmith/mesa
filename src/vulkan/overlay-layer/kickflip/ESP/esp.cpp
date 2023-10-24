#include "esp.hpp"

#include "../frame.hpp"
#include "../offsets.hpp"
#include "../utils.hpp"
#include "../cfg/config.hpp"
#include "../sdk/SDL.hpp"
#include "../sdk/classes/localplayer.hpp"
#include "../sdk/classes/csplayerpawn.hpp"
void ESP::OnFrame(){
  //TO HOOK OR NOT TO HOOK-> THAT IS THE QUESTION   
}


void ESP::DrawBoxes(){

}


void ESP::DrawBones(uintptr_t ptrpawn, Vector2 bounds, const view_matrix_t& matrix, bool isDude){
   LocalPlayer lp = LocalPlayer();
   CSPlayerPawn pawn = CSPlayerPawn(ptrpawn);
  
   if(!pawn.validate() || !lp.validate())
      return;

  
   BoneArray bones = BoneArray(pawn.bones());
   if(!bones.validate())
      return;
//https://www.unknowncheats.me/forum/3882591-post1663.html

   std::unordered_map<Bones::BONE, Vector3> skeleton;
 
   if(!bones.skeleton(skeleton))
      return;
   std::vector<Vector3> wb;
   Vector3 Ahead = Util::WorldToScreen(bounds, skeleton.at(Bones::HEAD), matrix); wb.push_back(Ahead);
   Vector3 Acou = Util::WorldToScreen(bounds, skeleton.at(Bones::COU), matrix); wb.push_back(Acou);
   Vector3 AshoulderR = Util::WorldToScreen(bounds, skeleton.at(Bones::RSHOULDER), matrix); wb.push_back(AshoulderR);
   Vector3 AshoulderL = Util::WorldToScreen(bounds, skeleton.at(Bones::LSHOULDER), matrix); wb.push_back(AshoulderL);
   Vector3 AbrasR = Util::WorldToScreen(bounds, skeleton.at(Bones::RBRAS), matrix); wb.push_back(AbrasR);
   Vector3 AbrasL = Util::WorldToScreen(bounds, skeleton.at(Bones::LBRAS), matrix); wb.push_back(AbrasL);
   Vector3 AhandR = Util::WorldToScreen(bounds, skeleton.at(Bones::RHAND), matrix); wb.push_back(AhandR);
   Vector3 AhandL = Util::WorldToScreen(bounds, skeleton.at(Bones::LHAND), matrix); wb.push_back(AhandL);
   Vector3 Acock = Util::WorldToScreen(bounds, skeleton.at(Bones::COCK), matrix); wb.push_back(Acock);
   Vector3 AkneesR = Util::WorldToScreen(bounds, skeleton.at(Bones::RKNEE), matrix); wb.push_back(AkneesR);
   Vector3 AkneesL = Util::WorldToScreen(bounds, skeleton.at(Bones::LKNEE), matrix); wb.push_back(AkneesL);
   Vector3 AfeetR = Util::WorldToScreen(bounds, skeleton.at(Bones::RFOOT), matrix); wb.push_back(AfeetR);
   Vector3 AfeetL = Util::WorldToScreen(bounds, skeleton.at(Bones::LFOOT), matrix); wb.push_back(AfeetL);

   for (const auto bone : wb)
   {
      //Vector3 bone = skeleton.at(b);
      if (bone.x == 0.f || bone.y == 0.f || bone.z == 0.f)
      {
         return;
      }
      if (bone.z < 0.01f) // higher?
         return;
   }
 
   float thickness = cfg->settings.ESP.Bones.Thickness;
   ImU32 clr = ImGui::ColorConvertFloat4ToU32(ImVec4(1, 1, 1, 1));

   int team = pawn.teamNum();
   bool is_defusing = pawn.defusing();
   if (team == Teams::T)
      clr = ImGui::ColorConvertFloat4ToU32(cfg->settings.ESP.Box.TColor);
   else if (team == Teams::CT && is_defusing)
      clr = ImGui::ColorConvertFloat4ToU32(cfg->settings.ESP.Box.DColor);
   else if (team == Teams::CT && !is_defusing)
      clr = ImGui::ColorConvertFloat4ToU32(cfg->settings.ESP.Box.CTColor);


   if (kf->getAim()->ShouldHighlight() > 0 ) {
      if(pawn.addy() == kf->getAim()->ShouldHighlight() && lp.isAlive())
         clr = ImGui::ColorConvertFloat4ToU32(ImVec4(1,0,0,1));
           // ImGui::GetOverlayDrawList()->AddLine(ImVec2(punchX - 5, punchY), ImVec2(punchX + 5, punchY), clr, 3);
           // ImGui::GetOverlayDrawList()->AddLine(ImVec2(punchX, punchY - 5), ImVec2(punchX, punchY + 5), clr, 3);
   }
   bool didBreak = false;
   if(float dist = distance(ImVec2(Acou.x, Acou.y), ImVec2(Ahead.x, Ahead.y)); dist  > cfg->settings.ESP.Bones.DebugDistance){
      kf->LogBone(xs("broke HCou bone at "), dist); didBreak = true;
   }
   if(float dist = distance(ImVec2(Acou.x, Acou.y), ImVec2(AshoulderR.x, AshoulderR.y)); dist  > cfg->settings.ESP.Bones.DebugDistance){
      kf->LogBone(xs("broke CouSR bone at "), dist); didBreak = true;
   }
   if(float dist = distance(ImVec2(Acou.x, Acou.y), ImVec2(AshoulderL.x, AshoulderL.y)); dist  > cfg->settings.ESP.Bones.DebugDistance){
      kf->LogBone(xs("broke CouSL bone at "), dist); didBreak = true;
   }
   if(float dist = distance(ImVec2(AbrasL.x, AbrasL.y), ImVec2(AshoulderL.x, AshoulderL.y)); dist  > cfg->settings.ESP.Bones.DebugDistance){
      kf->LogBone(xs("broke Lb Ls bone at "), dist); didBreak = true;
   }
   if(float dist = distance(ImVec2(AbrasR.x, AbrasR.y), ImVec2(AshoulderR.x, AshoulderR.y)); dist  > cfg->settings.ESP.Bones.DebugDistance){
      kf->LogBone(xs("broke Rb Rs bone at "), dist); didBreak = true;
   }
   if(float dist = distance(ImVec2(AbrasR.x, AbrasR.y), ImVec2(AhandR.x, AhandR.y)); dist  > cfg->settings.ESP.Bones.DebugDistance){
      kf->LogBone(xs("broke RbRh bone at "), dist); didBreak = true;
   }
    if(float dist = distance(ImVec2(AbrasL.x, AbrasL.y), ImVec2(AhandL.x, AhandL.y)); dist  > cfg->settings.ESP.Bones.DebugDistance){
      kf->LogBone(xs("broke LbLh bone at "), dist); didBreak = true;
   }
    if(float dist = distance(ImVec2(Acou.x, Acou.y), ImVec2(Acock.x, Acock.y)); dist  > cfg->settings.ESP.Bones.DebugDistance){
      kf->LogBone(xs("broke CockCou bone at "), dist); didBreak = true;
   }
    if(float dist = distance(ImVec2(AkneesR.x, AkneesR.y), ImVec2(Acock.x, Acock.y)); dist  > cfg->settings.ESP.Bones.DebugDistance){
      kf->LogBone(xs("broke cockRk bone at "), dist); didBreak = true;
   }
    if(float dist = distance(ImVec2(AkneesL.x, AkneesL.y), ImVec2(Acock.x, Acock.y)); dist  > cfg->settings.ESP.Bones.DebugDistance){
      kf->LogBone(xs("broke cockLk bone at "), dist); didBreak = true;
   }
    if(float dist = distance(ImVec2(AkneesL.x, AkneesL.y), ImVec2(AfeetL.x, AfeetL.y)); dist  > cfg->settings.ESP.Bones.DebugDistance){
      kf->LogBone(xs("broke LkLf bone at "), dist); didBreak = true;
   }
    if(float dist = distance(ImVec2(AkneesR.x, AkneesR.y), ImVec2(AfeetR.x, AfeetR.y)); dist  > cfg->settings.ESP.Bones.DebugDistance){
      kf->LogBone(xs("broke RkRf bone at "), dist); didBreak = true;
   }
   if(didBreak)
      return;

   ImGui::GetOverlayDrawList()->AddLine(ImVec2(Acou.x, Acou.y), ImVec2(Ahead.x, Ahead.y), clr, thickness);
   //if dist (imvec2 imvec2) > threshold didBreak = true;
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(Acou.x, Acou.y), ImVec2(AshoulderR.x, AshoulderR.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(Acou.x, Acou.y), ImVec2(AshoulderL.x, AshoulderL.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(AbrasL.x, AbrasL.y), ImVec2(AshoulderL.x, AshoulderL.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(AbrasR.x, AbrasR.y), ImVec2(AshoulderR.x, AshoulderR.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(AbrasR.x, AbrasR.y), ImVec2(AhandR.x, AhandR.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(AbrasL.x, AbrasL.y), ImVec2(AhandL.x, AhandL.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(Acou.x, Acou.y), ImVec2(Acock.x, Acock.y), clr, thickness);

   if(cfg->settings.ESP.Bones.Cocks){
      Vector3 cockPos = skeleton.at(Bones::LHAND); 
      cockPos.z = skeleton.at(Bones::COCK).z; 
      cockPos.x = (skeleton.at(Bones::LHAND).x + skeleton.at(Bones::RHAND).x) / 2.f; 
      cockPos.y = (skeleton.at(Bones::LHAND).y + skeleton.at(Bones::RHAND).y) / 2.f;
      float cock_thickness = thickness + 0.25f;
      if(isDude){
        cockPos.x = cockPos.x * 1.05;
        cockPos.y = cockPos.y * 1.05;
        cock_thickness += 2.f;
      }
      Vector3 LongCock = Util::WorldToScreen(bounds, cockPos, matrix);
      ImGui::GetOverlayDrawList()->AddLine(ImVec2(Acock.x, Acock.y), ImVec2(LongCock.x, LongCock.y), clr, cock_thickness);
   }
   
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(AkneesR.x, AkneesR.y), ImVec2(Acock.x, Acock.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(AkneesL.x, AkneesL.y), ImVec2(Acock.x, Acock.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(AkneesL.x, AkneesL.y), ImVec2(AfeetL.x, AfeetL.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(AkneesR.x, AkneesR.y), ImVec2(AfeetR.x, AfeetR.y), clr, thickness);
  
}//end DrawBones