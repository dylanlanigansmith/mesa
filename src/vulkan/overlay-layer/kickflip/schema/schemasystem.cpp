#include "../includes.hpp"
#include "../frame.hpp"
#include "schemasystem.hpp"


#include "../globals.hpp"
#include "../mem/memory.hpp"

#include "../mem/virtual.hpp"


CSchemaSystem* CSchemaSystem::Get() {
    static const CPointer inst = kf->GetMem()->GetInterface(SCHEMA_LIBX, xc("SchemaSystem_001"));
    return inst.Get<CSchemaSystem*>();
}

CSchemaSystemTypeScope* CSchemaSystem::FindTypeScopeForModule(const char* name) {
    return vt::CallMethod<CSchemaSystemTypeScope*>(this, 13, name, nullptr);
}

CSchemaClassInfo* CSchemaSystemTypeScope::FindDeclaredClass(const char* name) {
    CSchemaClassInfo* binding = nullptr;


    binding = vt::CallMethod<CSchemaClassInfo*>(this, 2, name);


    return binding;
}

bool CSchemaClassInfo::InheritsFrom(CSchemaClassInfo* other) {
    if (!other || !m_BaseClasses) return false;

    if (this == other) return true;

    for (int i = 0; i < m_nBaseClassesCount; ++i) {
        auto& baseClass = m_BaseClasses[0];
        if (baseClass.m_pClass->InheritsFrom(other)) {
            return true;
        }
    }

    return false;
}