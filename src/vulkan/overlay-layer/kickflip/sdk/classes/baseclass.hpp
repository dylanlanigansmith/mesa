#pragma once
#include "../../includes.hpp"
#include "../sdk.hpp"
#include "../../mem/memory.hpp"
#include "../../frame.hpp"
#include "../../offsets.hpp"

#include "../../s2_types.hpp"


class CBase  { //abstract-esque
    public:
        CBase(){
            valid = false;
            hasChecked = false;
        }
        CBase(uintptr_t h){
            CBase();
            handle = h;
            validate();
        }
        ~CBase(){}

        bool validate(){
            hasChecked = true;
            if(handle > 0)
                valid = true;
            return valid;
        }
        uintptr_t addy(){ return 0;}
    protected:
        uintptr_t handle;
        bool hasChecked;
        bool valid;
};