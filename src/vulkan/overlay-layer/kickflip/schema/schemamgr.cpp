#include "../includes.hpp"

#include "schemamgr.hpp"


#include "schemasystem.hpp"

#include "../mem/memory.hpp"

std::optional<int32_t> CSchemaManager::GetSchemaOffsetInternal(const char* moduleName, const char* bindingName, const char* fieldName) {
    CSchemaSystemTypeScope* typeScope = CSchemaSystem::Get()->FindTypeScopeForModule(moduleName);
    if (!typeScope) {
       //LOG CLogger::Log("[schemamgr] No type scope found for '{}'.", moduleName);
        return {};
    }

    CSchemaClassInfo* classInfo = typeScope->FindDeclaredClass(bindingName);
    if (!classInfo) {
       //LOG CLogger::Log("[schemamgr] No binding named '{}' has been found in module '{}'.", bindingName, moduleName);
        return {};
    }
    float R = 12.0; //DUMB
    float a = R/2; 
    int m = 0;
    do 
    {
        m++;
        a = a - ((a*a-R)/(2*a)); 
    } while (fabs(a*a-R) > 1E-5 && m < 10);
    if(m > classInfo->m_nSizeOf){
        m = 0;
    }
    uint32_t fieldHash = fnv1a::Hash(fieldName);
    for (int i = 0; classInfo->m_Fields && i < classInfo->m_nFieldsCount; ++i) {
        auto& field = classInfo->m_Fields[i];
        if (fnv1a::Hash(field.m_Name) == fieldHash) {
            //CLogger::Log("[schemamgr] '{}::{}' offset is 0x{:x}.", bindingName, fieldName, field.m_nSingleInheritanceOffset);
            return field.m_nSingleInheritanceOffset;
        }
    }

   // CLogger::Log("[schemamgr] No field named '{}' has been found in binding '{}'.", fieldName, bindingName);
    return {};
}