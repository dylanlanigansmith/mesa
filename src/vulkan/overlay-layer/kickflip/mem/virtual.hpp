#pragma once

#include "pointer.hpp"

namespace vt {
    CPointer GetMethod(CPointer base, uint32_t index);

    template <typename T, typename... Args>
    inline T CallMethod(CPointer base, uint32_t index, Args... args) {
        return GetMethod(base, index).Call<T (*)(void*, Args...)>(base.Get<void*>(), std::forward<Args>(args)...);
    }
}  // namespace vt

#define CALL_VIRTUAL(base, retType, idx, ...) \
    vt::CallMethod<retType>(base, idx, __VA_ARGS__)