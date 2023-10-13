#include "esp.hpp"

#include "../frame.hpp"
#include "../offsets.hpp"
#include "../utils.hpp"
#include "../cfg/config.hpp"
#include "../sdk/SDL.hpp"
void ESP::OnFrame(){
  //TO HOOK OR NOT TO HOOK-> THAT IS THE QUESTION   
}

void ESP::DrawBones(uintptr_t pawn, uintptr_t lp, Vector2 bounds, view_matrix_t matrix, int team, bool is_defusing)
{
   
   uintptr_t gamescenenode = *(uintptr_t *)(pawn + GameSceneNode);
   if (!gamescenenode)
      return;
   uintptr_t bonearray = *(uintptr_t *)(gamescenenode + CModelState + oBoneArray);
   Vector3 head = *(Vector3 *)(bonearray + 6 * 32);
   Vector3 cou = *(Vector3 *)(bonearray + 5 * 32);
   Vector3 shoulderR = *(Vector3 *)(bonearray + 8 * 32);
   Vector3 shoulderL = *(Vector3 *)(bonearray + 13 * 32);
   Vector3 brasR = *(Vector3 *)(bonearray + 9 * 32);
   Vector3 brasL = *(Vector3 *)(bonearray + 14 * 32);
   Vector3 handR = *(Vector3 *)(bonearray + 11 * 32);
   Vector3 handL = *(Vector3 *)(bonearray + 16 * 32);
   Vector3 cock = *(Vector3 *)(bonearray + 0 * 32);
   Vector3 kneesR = *(Vector3 *)(bonearray + 23 * 32);
   Vector3 kneesL = *(Vector3 *)(bonearray + 26 * 32);
   Vector3 feetR = *(Vector3 *)(bonearray + 24 * 32);
   Vector3 feetL = *(Vector3 *)(bonearray + 27 * 32);

   std::vector<Vector3> bones;

   //todo:
   //check if dist btwn pts is really long
   //maybe dist btwn head to feet vs hands to chest 
   //check if lines make sense (not screen width)
   Vector3 Ahead = Util::WorldToScreen(bounds, head, matrix);
   bones.push_back(Ahead);
   Vector3 Acou = Util::WorldToScreen(bounds, cou, matrix);
   bones.push_back(Acou);
   Vector3 AshoulderR = Util::WorldToScreen(bounds, shoulderR, matrix);
   bones.push_back(AshoulderR);
   Vector3 AshoulderL = Util::WorldToScreen(bounds, shoulderL, matrix);
   bones.push_back(AshoulderL);
   Vector3 AbrasR = Util::WorldToScreen(bounds, brasR, matrix);
   bones.push_back(AbrasR);
   Vector3 AbrasL = Util::WorldToScreen(bounds, brasL, matrix);
   bones.push_back(AbrasL);
   Vector3 AhandR = Util::WorldToScreen(bounds, handR, matrix);
   bones.push_back(AhandR);
   Vector3 AhandL = Util::WorldToScreen(bounds, handL, matrix);
   bones.push_back(AhandL);
   Vector3 Acock = Util::WorldToScreen(bounds, cock, matrix);
   bones.push_back(Acock);
   Vector3 AkneesR = Util::WorldToScreen(bounds, kneesR, matrix);
   bones.push_back(AkneesR);
   Vector3 AkneesL = Util::WorldToScreen(bounds, kneesL, matrix);
   bones.push_back(AkneesL);
   Vector3 AfeetR = Util::WorldToScreen(bounds, feetR, matrix);
   bones.push_back(AfeetR);
   Vector3 AfeetL = Util::WorldToScreen(bounds, feetL, matrix);
   bones.push_back(AfeetL);

   for (auto &bone : bones)
   {
      if (bone.x == 0.f || bone.y == 0.f || bone.z == 0.f)
      {
         return;
      }
      if (bone.z < 0.01f) // higher?
         return;
   }
   float thickness = cfg->settings.ESP.Bones.Thickness;
   ImU32 clr = ImGui::ColorConvertFloat4ToU32(ImVec4(1, 1, 1, 1));
   if (team == 2)
      clr = ImGui::ColorConvertFloat4ToU32(cfg->settings.ESP.Box.TColor);
   else if (team == 3 && is_defusing)
      clr = ImGui::ColorConvertFloat4ToU32(cfg->settings.ESP.Box.DColor);
   else if (team == 3 && !is_defusing)
      clr = ImGui::ColorConvertFloat4ToU32(cfg->settings.ESP.Box.CTColor);
   if (kf->getAim()->ShouldHighlight() > 0){
      if(pawn == kf->getAim()->ShouldHighlight())
         clr = ImGui::ColorConvertFloat4ToU32(ImVec4(1,0,0,1));
           // ImGui::GetOverlayDrawList()->AddLine(ImVec2(punchX - 5, punchY), ImVec2(punchX + 5, punchY), clr, 3);
                // ImGui::GetOverlayDrawList()->AddLine(ImVec2(punchX, punchY - 5), ImVec2(punchX, punchY + 5), clr, 3);
   }
   
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(Acou.x, Acou.y), ImVec2(Ahead.x, Ahead.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(Acou.x, Acou.y), ImVec2(AshoulderR.x, AshoulderR.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(Acou.x, Acou.y), ImVec2(AshoulderL.x, AshoulderL.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(AbrasL.x, AbrasL.y), ImVec2(AshoulderL.x, AshoulderL.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(AbrasR.x, AbrasR.y), ImVec2(AshoulderR.x, AshoulderR.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(AbrasR.x, AbrasR.y), ImVec2(AhandR.x, AhandR.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(AbrasL.x, AbrasL.y), ImVec2(AhandL.x, AhandL.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(Acou.x, Acou.y), ImVec2(Acock.x, Acock.y), clr, thickness);
   if(cfg->settings.ESP.Bones.Cocks){
    Vector3 cockPos = handL; cockPos.z = cock.z; cockPos.x = (handL.x + handR.x) / 2.f; cockPos.y = (handL.y + handR.y) / 2.f;
    Vector3 LongCock = Util::WorldToScreen(bounds, cockPos, matrix);
    ImGui::GetOverlayDrawList()->AddLine(ImVec2(Acock.x, Acock.y), ImVec2(LongCock.x, LongCock.y), clr, thickness + 0.25f);
   }
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(AkneesR.x, AkneesR.y), ImVec2(Acock.x, Acock.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(AkneesL.x, AkneesL.y), ImVec2(Acock.x, Acock.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(AkneesL.x, AkneesL.y), ImVec2(AfeetL.x, AfeetL.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(AkneesR.x, AkneesR.y), ImVec2(AfeetR.x, AfeetR.y), clr, thickness);
}

void ESP::DrawBoxes(){

}


/*

void StickFigure(uintptr_t CSPlayerPawn, Vector2 bounds, view_matrix_t matrix, int team, bool is_defusing)
{

   uintptr_t gamescenenode = *(uintptr_t *)(CSPlayerPawn + GameSceneNode);
   if (!gamescenenode)
      return;
   uintptr_t bonearray = *(uintptr_t *)(gamescenenode + CModelState + BoneArray);
   Vector3 head = *(Vector3 *)(bonearray + 6 * 32);
   Vector3 cou = *(Vector3 *)(bonearray + 5 * 32);
   Vector3 shoulderR = *(Vector3 *)(bonearray + 8 * 32);
   Vector3 shoulderL = *(Vector3 *)(bonearray + 13 * 32);
   Vector3 brasR = *(Vector3 *)(bonearray + 9 * 32);
   Vector3 brasL = *(Vector3 *)(bonearray + 14 * 32);
   Vector3 handR = *(Vector3 *)(bonearray + 11 * 32);
   Vector3 handL = *(Vector3 *)(bonearray + 16 * 32);
   Vector3 cock = *(Vector3 *)(bonearray + 0 * 32);
   Vector3 kneesR = *(Vector3 *)(bonearray + 23 * 32);
   Vector3 kneesL = *(Vector3 *)(bonearray + 26 * 32);
   Vector3 feetR = *(Vector3 *)(bonearray + 24 * 32);
   Vector3 feetL = *(Vector3 *)(bonearray + 27 * 32);

   std::vector<Vector3> bones;

   //todo:
   //check if dist btwn pts is really long
   //maybe dist btwn head to feet vs hands to chest 
   //check if lines make sense (not screen width)
   Vector3 Ahead = Util::WorldToScreen(bounds, head, matrix);
   bones.push_back(Ahead);
   Vector3 Acou = Util::WorldToScreen(bounds, cou, matrix);
   bones.push_back(Acou);
   Vector3 AshoulderR = Util::WorldToScreen(bounds, shoulderR, matrix);
   bones.push_back(AshoulderR);
   Vector3 AshoulderL = Util::WorldToScreen(bounds, shoulderL, matrix);
   bones.push_back(AshoulderL);
   Vector3 AbrasR = Util::WorldToScreen(bounds, brasR, matrix);
   bones.push_back(AbrasR);
   Vector3 AbrasL = Util::WorldToScreen(bounds, brasL, matrix);
   bones.push_back(AbrasL);
   Vector3 AhandR = Util::WorldToScreen(bounds, handR, matrix);
   bones.push_back(AhandR);
   Vector3 AhandL = Util::WorldToScreen(bounds, handL, matrix);
   bones.push_back(AhandL);
   Vector3 Acock = Util::WorldToScreen(bounds, cock, matrix);
   bones.push_back(Acock);
   Vector3 AkneesR = Util::WorldToScreen(bounds, kneesR, matrix);
   bones.push_back(AkneesR);
   Vector3 AkneesL = Util::WorldToScreen(bounds, kneesL, matrix);
   bones.push_back(AkneesL);
   Vector3 AfeetR = Util::WorldToScreen(bounds, feetR, matrix);
   bones.push_back(AfeetR);
   Vector3 AfeetL = Util::WorldToScreen(bounds, feetL, matrix);
   bones.push_back(AfeetL);

   for (auto &bone : bones)
   {
      if (bone.x == 0.f || bone.y == 0.f || bone.z == 0.f)
      {
         return;
      }
      if (bone.z < 0.01f) // higher?
         return;
   }
   float thickness = 1.f;
   ImU32 clr = ImGui::ColorConvertFloat4ToU32(ImVec4(1, 1, 1, 1));
   if (team == 2)
      clr = ImGui::ColorConvertFloat4ToU32(box_color_t);
   else if (team == 3 && is_defusing)
      clr = ImGui::ColorConvertFloat4ToU32(box_color_defuse);
   else if (team == 3 && !is_defusing)
      clr = ImGui::ColorConvertFloat4ToU32(box_color_ct);

   if((1200 < Ahead.x < 1400) || (600 < Ahead.y < 800)){
      if (team == 2 ){
         //https://tronche.com/gui/x/xlib/input/XWarpPointer.html
      //   Display *display = XOpenDisplay(NULL);
     //  assert(display);
     // XSetErrorHandler(_XlibErrorHandler);
     // Window focused;
    //  int revert_to;
      //XWarpPointer(display, src_w, dest_w, src_x, src_y, src_width, src_height, dest_x, dest_y)
                
      // XGetInputFocus(display, &focused, &revert_to);  

       //XWarpPointer(display, None,None, 0,0,0,0, Ahead.x,Ahead.y);
       //XCloseDisplay(display);
      }
      
   }
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(Acou.x, Acou.y), ImVec2(Ahead.x, Ahead.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(Acou.x, Acou.y), ImVec2(AshoulderR.x, AshoulderR.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(Acou.x, Acou.y), ImVec2(AshoulderL.x, AshoulderL.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(AbrasL.x, AbrasL.y), ImVec2(AshoulderL.x, AshoulderL.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(AbrasR.x, AbrasR.y), ImVec2(AshoulderR.x, AshoulderR.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(AbrasR.x, AbrasR.y), ImVec2(AhandR.x, AhandR.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(AbrasL.x, AbrasL.y), ImVec2(AhandL.x, AhandL.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(Acou.x, Acou.y), ImVec2(Acock.x, Acock.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(AkneesR.x, AkneesR.y), ImVec2(Acock.x, Acock.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(AkneesL.x, AkneesL.y), ImVec2(Acock.x, Acock.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(AkneesL.x, AkneesL.y), ImVec2(AfeetL.x, AfeetL.y), clr, thickness);
   ImGui::GetOverlayDrawList()->AddLine(ImVec2(AkneesR.x, AkneesR.y), ImVec2(AfeetR.x, AfeetR.y), clr, thickness);
}
*/