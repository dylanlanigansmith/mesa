#pragma once
#include "baseclass.hpp"

class CollisionProperty : public CBase
{
public:
    using CBase::CBase;

    uint16_t CollisionMask()
    {
        return *(uint16_t *)(handle + 0x38); // sick
    }
    Vector3 vecMins()
    {
        Vector3 ret;
        ret.x = 0;
        ret.y = 0;
        ret.z = 0;
        if (!m_vecMins)
            m_vecMins = kf->off->get(xs("CCollisionProperty"), xs("m_vecMins"));
        if (!m_vecMins)
            return ret;

        ret = *(Vector3 *)(handle + m_vecMins);
    }
    Vector3 VecMaxs()
    {
        Vector3 ret;
        ret.x = 0;
        ret.y = 0;
        ret.z = 0;
        if (!m_vecMaxs)
            m_vecMaxs = kf->off->get(xs("CCollisionProperty"), xs("m_vecMins"));
        if (!m_vecMaxs)
            return ret;

        ret = *(Vector3 *)(handle + m_vecMaxs);
    }
    uint16_t solidFlags()
    {
        if (!m_usSolidFlags)
            m_usSolidFlags = kf->off->get(xs("CCollisionProperty"), xs("m_usSolidFlag"));
        if (!m_usSolidFlags)
            return 0;
        uint16_t fgs = *(uint16_t*)(handle + m_usSolidFlags);
        return fgs;
    }

public:
    static uintptr_t m_vecMins;
    static uintptr_t m_vecMaxs;
    static uintptr_t m_usSolidFlags;
};