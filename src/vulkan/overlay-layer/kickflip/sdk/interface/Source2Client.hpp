#pragma once
#include <stdint.h>
class CSource2Client {
   public:
    static  CSource2Client* Get();
    uintptr_t addy(){
        return (uintptr_t)this;
    }
   virtual uintptr_t FrameStageNotify(uintptr_t a1, int32_t stage);
};