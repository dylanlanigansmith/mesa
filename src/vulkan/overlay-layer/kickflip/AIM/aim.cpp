
#include "../frame.hpp"
#include "../cfg/config.hpp"
#include "../utils.hpp"
#include "../sdk/classes/localplayer.hpp"
#include "../sdk/enginetrace.hpp"
#include "aim.hpp"
AIM::AIM()
{
    state = aims::OFF;
    bounds.x = 2560;
    bounds.y = 1440;
    t_main = std::thread(&AIM::run, this);

    highlight = 0;
    shouldHighlight = false;
    sens = 0;
    lastLockSuccess = false;
}

AIM::~AIM()
{
    state = aims::SHUTDOWN;
    t_main.join();
}
class target
{
public:
    target(uintptr_t ply, uintptr_t p, uintptr_t b, Vector2 h, int d)
    {
        player = ply;
        pawn = p;
        bone = b;
        head_s = h;
        dist = d;
    }
    ~target() {}
    uintptr_t player;
    uintptr_t pawn;
    uintptr_t bone;
    Vector2 head_s;
    int dist;
};

void AIM::tick(moduleptr_t &cl)
{
    // We are in game

    LocalPlayer lp = LocalPlayer(cl);
    if (!lp.validate())
        return;

    if (!lp.isAlive())
        return;

    highlight = 0;
    shouldHighlight = false;
    uintptr_t entity_list = *(uintptr_t *)(cl->start + kf->EntityList());
    if (!entity_list)
        return;
    int key = cfg->settings.AIM.mouse4 ? 4 : 5;

    if(!kf->GetInput()->Mouse(key))
       lastLockSuccess = false;
    // if (kf->GetInput()->Mouse(key) ) //soooo the entire mouse system is fundamentally flawed....
    // {

    // GET A TARGET
    std::vector<target> targets;      // custom type tbh
    for (int i = 1; i < ENT_MAX; i++) // should we get targets first
    {
        if (kf->GetInput()->Mouse(key) && state == aims::HAS_TARGET && cfg->settings.AIM.single_target)
            break;

        // if(state != aims::IDLE)
        //   break;
        if (!lp.isAlive())
            break;
        PlayerController player = PlayerController(PlayerController::GetPlayerFromIndex(entity_list, i));

        if (!player.validate())
            continue;
        if (!player.isAlive())
            continue;

        if (player.team() < Teams::ALL || player.team() > Teams::CT)
            continue;
        if (player.team() == lp.team()) // so shouldnt need to check if localplayer
            continue;

        CSPlayerPawn pawn = CSPlayerPawn(player.pawn(entity_list));

        if (!pawn.validate())
            continue;
        if (!(pawn.flags() & FL_ONGROUND) && !cfg->settings.AIM.jumping)
            continue;

        BoneArray bones(pawn.bones());
        if (!bones.validate())
            continue;

        Vector3 head = bones.getBone(Bones::HEAD);
        if ((head.x == 0.f || head.y == 0.f || head.z == 0.f)) // untested
            continue;
      
        view_matrix_t matrix = *(view_matrix_t *)(cl->start + kf->ViewMatrix());
        Vector3 Ahead = Util::WorldToScreen(bounds, head, matrix); // a lot of memory to copy
        if (Ahead.z < 0.01)
            continue;
        if ((Ahead.x == 0.f || Ahead.y == 0.f)) // untested
            continue;

        CSPlayerPawn lpp = CSPlayerPawn(lp.pawn(entity_list));
        if (!lpp.validate())
            continue;
        float x_center = bounds.x / 2;
        float y_center = bounds.y / 2;

        int fov = cfg->settings.AIM.fov / 2;
        if ((cfg->settings.AIM.fov % 2) > 0)
            fov++;
        if (!(((x_center - fov) < Ahead.x && Ahead.x < (x_center + fov)) && ((y_center - fov) < Ahead.y && Ahead.y < (y_center + fov))))
            continue;
        // IN FOV
     

        Vector2 head2;
        head2.x = Ahead.x;
        head2.y = Ahead.y;
        int dist = sqrt(pow(head2.x - x_center, 2) + pow(head2.y - y_center, 2));

        state = aims::HAS_TARGET;
        targets.push_back(target(player.addy(), pawn.addy(), bones.addy(), head2, dist));
       
        if (cfg->settings.AIM.single_target) // what happens without this...
            break;
        //  if(targets.size() > 2)
        //     break;
    }
    while (kf->GetInput()->Mouse(key) ) // i jjust want to say this is getting out of hand state == aims::HAS_TARGET
    {
        if(lastLockSuccess && (cfg->settings.AIM.single_target))
            return;
        if(!(state == aims::AT_TARGET || state == aims::HAS_TARGET))
            break;

        

        int best_dist = 0xFFFF; // distance
        int best_idx = -1;
        int best_health; // number
        auto *front = &targets.front();

        target &best = targets.front();
        
        if(targets.size() == 0 )
            break;
        for (auto &t : targets) // assess targets for best one
        {

            PlayerController tplayer(t.player);
            CSPlayerPawn tpawn(t.pawn);
            BoneArray tbones(t.bone);

            if (!tplayer.validate() || !tpawn.validate() || !tbones.validate())
                continue;
            if (!tplayer.isAlive())
                continue;
            if (t.dist < best_dist)
            {
                best_dist = t.dist;
                auto index = std::distance(front, &t);
                best_idx = index;
                best = t;
                if (targets.size() == 1)
                    break;
            }

        } // end iterate thru targets
        if (best_idx == -1 || best_dist == 0)
        {
            state = aims::IDLE;
            return; // will u regret this
        }

        PlayerController player(best.player);

        CSPlayerPawn pawn(best.pawn);
        BoneArray bones(best.bone);
       
       
         if (!lp.validate())
            break;
        if (!player.isAlive() || !lp.isAlive())
        {
            state = aims::IDLE; // we killed em
            if (cfg->settings.AIM.log)
                kf->Log("we killed em");
            shouldHighlight = false;
            highlight = 0;
            lastLockSuccess = true;
            break;
        }
        shouldHighlight = true; //did moving this up work?
        highlight = pawn.addy();
        Bones::BONE goal_bone = Bones::HEAD;
        

        if (cfg->settings.AIM.randombone)
        {
            int coin = rand() % 100 + 1;
            if (cfg->settings.AIM.randombonechance > coin)
                goal_bone = Bones::COU;
        }
        else if (cfg->settings.AIM.body)
            goal_bone = Bones::COU;
        Vector3 head = bones.getBone(goal_bone);
        if ((head.x == 0.f || head.y == 0.f || head.z == 0.f)) // untested
            continue;
        view_matrix_t matrix = *(view_matrix_t *)(cl->start + kf->ViewMatrix());
        Vector3 Ahead = Util::WorldToScreen(bounds, head, matrix); // a lot of memory to copy
        if (Ahead.z < 0.01)
            continue;
        if ((Ahead.x == 0.f || Ahead.y == 0.f)) // untested
            continue;

        if (!cfg->settings.AIM.Enabled)
            break;
        
        lp.validate();
        CSPlayerPawn lp_pawn = CSPlayerPawn(lp.pawn(entity_list));
        if (!lp_pawn.validate())
            continue;
        sens = cfg->settings.AIM.sens;

        CWeapon lp_weapon = lp_pawn.clippingWeapon();
        if (!lp_weapon.validate())
            continue; // this bad
        WeaponType type = lp_weapon.type();

        if (!cfg->settings.AIM.Weapons.Pistols && type == PISTOL)
            break;
        if (!cfg->settings.AIM.Weapons.Rifles && type == RIFLESMG)
            break;
        if (!cfg->settings.AIM.Weapons.Shotguns && type == SHOTGUN) // i hate code like this
            break;
        if (!cfg->settings.AIM.Weapons.Scoped && type == SCOPED)
            break;
        Vector3 startPos = lp_pawn.eyePosition();
        Vector3 endPos = head;
        C_GameTrace trace;
        Util::VisibilityTrace(&trace, &startPos, &endPos, lp_pawn.addy());
        if (!trace.HitEntity)
            continue;

        std::string fr = std::to_string(trace.Fraction);
        // kf->Log( fr);
        if (trace.Fraction < 0.97f)
            continue;

        CSPlayerPawn hit = CSPlayerPawn(trace.HitEntity);
        if (GetHandleFromEntityRebuilt(hit.addy()) != GetHandleFromEntityRebuilt(pawn.addy()))
            continue;
        // Visible
        float x_center = bounds.x / 2;
        float y_center = bounds.y / 2;
        int hair_x = x_center;
        int hair_y = y_center;
        int punchX = hair_x;
        int punchY = hair_y;
        float perfection = 1.f;
        int dx = bounds.x / FOV;
        int dy = bounds.y / FOV;
        ImVec2 delta;
        ImVec2 pointer = ImVec2(x_center, y_center);
        if (kf->GetInput()->Mouse(1)) // necessary 
        {
            QAngle aimPunchAngle = lp_pawn.aimPunchAngle();
            if ((aimPunchAngle.x == 69.f || aimPunchAngle.y == 69.f))
                continue;

            aimPunchAngle.x *= cfg->settings.AIM.punchX / 100.f;
            aimPunchAngle.y *= cfg->settings.AIM.punchY / 100.f;
            if (cfg->settings.AIM.log)
                kf->Log("punch:", aimPunchAngle);
            punchX = (int)(hair_x - (dx * aimPunchAngle.y));
            punchY = (int)(hair_y + (dy * aimPunchAngle.x));
        }
        delta = ImVec2(Ahead.x - punchX, Ahead.y - punchY);

        if (!kf->GetVMouse())
            continue;
        float max_shift_x = bounds.x * ((float)cfg->settings.AIM.max_shift / 100.f);
        float max_shift_y = bounds.y * ((float)cfg->settings.AIM.max_shift / 100.f);
        if (delta.x > max_shift_x || delta.y > max_shift_y)
        {
            if (cfg->settings.AIM.log)
                kf->Log(std::to_string(max_shift_x) + "out of bounds");
            break;
        }
        int dist = sqrt(pow(delta.x, 2) + pow(delta.y, 2)); // asume origin
        if (dist > cfg->settings.AIM.max_dist)
        {
            if (cfg->settings.AIM.log)
                kf->Log(dist, "dist 2 big");
            break;
        }
        delta.x = delta.x / sens; // mouseSensitivity convar
        delta.y = delta.y / sens;

        state = aims::AT_TARGET;
        Vector2 pInicial = {0, 0}; // should this be punch?
        Vector2 pLastMove = {0, 0};
        Vector2 pMedio1;
        Vector2 pMedio2;
        Vector2 pFinal;

        pFinal.x = delta.x;
        pFinal.y = delta.y;

        if ((pFinal.x >= pInicial.x && pFinal.y >= pInicial.y) || (pFinal.x <= pInicial.x && pFinal.y <= pInicial.y))
        {
            pMedio1.x = pInicial.x + ((pFinal.x - pInicial.x) / getRnd(4, 8));
            pMedio1.y = pInicial.y + ((pFinal.x - pInicial.x) / getRnd(4, 8));

            pMedio2.x = pInicial.x + ((pFinal.x - pInicial.x) / getRnd(3, 6));
            pMedio2.y = pInicial.y + ((pFinal.x - pInicial.x) / getRnd(3, 6));
        }
        else if ((pFinal.x >= pInicial.x && pFinal.y <= pInicial.y) || (pFinal.x <= pInicial.x && pFinal.y >= pInicial.y))
        {
            pMedio1.x = pInicial.x + ((pFinal.x - pInicial.x) / getRnd(4, 8));
            pMedio1.y = pInicial.y - ((pFinal.x - pInicial.x) / getRnd(4, 8));

            pMedio2.x = pInicial.x + ((pFinal.x - pInicial.x) / getRnd(3, 5));
            pMedio2.y = pInicial.y - ((pFinal.x - pInicial.x) / getRnd(3, 5)); //play with this
        }
        Vector2 total = {0, 0};

        for (float i = 0; i < 1.01; i += 0.01)
        {
            int xa = getPt(pInicial.x, pMedio1.x, i);
            int ya = getPt(pInicial.y, pMedio1.y, i);
            int xb = getPt(pMedio1.x, pMedio2.x, i);
            int yb = getPt(pMedio1.y, pMedio2.y, i);
            int xc = getPt(pMedio2.x, pFinal.x, i);
            int yc = getPt(pMedio2.y, pFinal.y, i);

            int xm = getPt(xa, xb, i);
            int ym = getPt(ya, yb, i);
            int xn = getPt(xb, xc, i);
            int yn = getPt(yb, yc, i);

            int x = getPt(xm, xn, i);
            int y = getPt(ym, yn, i);

            Vector2 move = {x - pLastMove.x, y - pLastMove.y}; // Relative movement

            // YourMethodHere(move.x, move.y);

            kf->GetVMouse()->setmouse(move.x, move.y);
            std::this_thread::sleep_for(std::chrono::microseconds(cfg->settings.AIM.delay)); // 172
            // bugs:
            // oscillates between multiple targets.. toggle...
            // doesnt switch to body whensprayinh
            // weird offset related to viewpunch... reset
            pLastMove.x = x;
            pLastMove.y = y;
            state = aims::LOCKING; 
            /*
            *Need to highlight target...
                -need to select target but then stick to it
                -need to use key as toggle from the start for punch calc & take in to account mouse1 maybe
                -need to visualize FOV when aim tab open (this is smart af)
                -need to visualize punch angle too
            */
        }
    }
    // state = aims::IDLE;
    // end if target:
    // }
}

void AIM::run()
{
    kf->Log(xs("Aim: thread start"));
    while (state != aims::SHUTDOWN)
    {
        while (cfg->settings.AIM.Enabled && kf->GetMem()->Ready() && bounds.x > 0)
        {
            state = aims::IDLE;
            auto &cl = kf->GetMem()->GetModule(CLIENT_LIBX);
            if (!cl->start)
                break; // lets give it a sec ???? oh god

            if (!CEngineClient::Get()->IsInGame())
                break;
            if (!cfg->settings.AIM.Enabled)
                break;

            if (cfg->settings.AIM.Enabled)
                tick(cl);
        }

        state = aims::OFF;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    kf->Log(xs("Aim: thread stop"));
}

uintptr_t AIM::ShouldHighlight()
{
    if (!(highlight > 0) || !shouldHighlight)
        return 0;

    return highlight;
}

void AIM::reset()
{
    shouldHighlight = false;
    kf->Log(xs("Aim: sent shutdown signal"));
    state = aims::SHUTDOWN;
}