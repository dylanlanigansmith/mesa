#pragma once

#include "../../includes.hpp"
#include "../../s2_types.hpp"
namespace Bones
{
    enum BONE{
        HEAD = (6*32),
        COU = (5*32),
        RSHOULDER = (8*32),
        LSHOULDER = (13*32),
        RBRAS = (9*32),
        LBRAS = (14*32),
        RHAND = (11*32),
        LHAND = (16*32),
        COCK = (0*32),
        RKNEE = (23*32),
        LKNEE = (26*32),
        RFOOT = (24*32),
        LFOOT = (27*32)
    };
    static const BONE All[] = {HEAD, COU, RSHOULDER, LSHOULDER, RBRAS, LBRAS, RHAND, LHAND, COCK, RKNEE, LKNEE, RFOOT, LFOOT};
}

class BoneArray{
public:
    BoneArray(uintptr_t h){
        valid = false;
        checked = false;
        handle = h;
        validate();

    }
    bool skeleton(std::unordered_map<Bones::BONE, Vector3> &skel){
        if(!validate())
            return false;
        for(const auto b: Bones::All){
            Vector3 pos = getBone(b);
            if(pos.x && pos.y && pos.z)
                skel.insert({b, pos});
        }
        return (skel.size() >= 13);

    }
    Vector3 getBone(Bones::BONE idx){
        if(!valid)
            return Vector3{};
        Vector3 pos = *(Vector3*)(handle + idx);
        return pos;
    }
    bool validate(){
        checked = true;
        if(handle > 0)
            valid = true;
        return valid;
    }
    uintptr_t addy(){ //swing the addy fam
            return handle;
    }
private:
    bool valid;
    bool checked;
    uintptr_t handle;
};