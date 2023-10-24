#pragma once
#include <stdint.h>
#include "../../mem/virtual.hpp"
//#include "../econ/ceconitemsystem.hpp"
class CSource2Client {
   public:
    static  CSource2Client* Get();
    uintptr_t addy(){
        return (uintptr_t)this;
    }
  /* virtual uintptr_t FrameStageNotify(uintptr_t a1, int32_t stage);
    auto GetEconItemSystem() {
        return CALL_VIRTUAL(this, CEconItemSystem*, 111, this);
    }*/
};