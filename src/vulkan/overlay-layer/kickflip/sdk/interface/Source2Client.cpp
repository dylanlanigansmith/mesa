#include "Source2Client.hpp"
#include "../../mem/memory.hpp"
#include "../../frame.hpp"
#include "../../mem/virtual.hpp"
CSource2Client* CSource2Client::Get() {
    static const auto inst = kf->GetMem()->GetInterface(CLIENT_LIBX, xc("Source2Client002"));
    return inst.Get<CSource2Client*>();
}

