#include "IEngineClient.hpp"
#include "../../mem/memory.hpp"
#include "../../frame.hpp"
#include "../../mem/virtual.hpp"
CEngineClient* CEngineClient::Get() {
    static const auto inst = kf->GetMem()->GetInterface(ENGINE_LIBX, xc("Source2EngineToClient001"));
    return inst.Get<CEngineClient*>();
}

bool CEngineClient::IsInGame() { return vt::CallMethod<bool>(this, 32); }

int CEngineClient::GetLocalPlayer() {
    int index = -1;


    index = vt::CallMethod<int>(this, 44, 0);


    return index + 1;
}

int CEngineClient::GetEngineBuildNumber() { return vt::CallMethod<int>(this, 76); }