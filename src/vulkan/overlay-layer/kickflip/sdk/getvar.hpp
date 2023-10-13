#pragma once
#include "../schema/schemamgr.hpp"
#include "../utils.hpp"

#include "../includes.hpp"
class VarMgr {
public:
    VarMgr(){
        cache.insert({"null",0}); calls = 0;
    }
    ~VarMgr(){

    }/*
    uintptr_t get(std::string clss, std::string field){
        uintptr_t offset = 0;
        if(cache.find(clss.append(field)) == cache.end()){
        
            offset = CSchemaManager::GetSchemaOffset(Constants::CLIENT_LIB,clss.c_str(),field.c_str()).value();
            if(offset){
                cache.emplace(std::pair<std::string, uintptr_t>(clss.append(field), offset));
                return offset;
            }
               
           return 0;
        }
          else{
            return cache.at(clss.append(field));
        }
        return 0;
        
    }
    uintptr_t get(const char* lib, std::string clss, std::string field){\
             uintptr_t offset = 0;
        if(cache.find(clss.append(field)) == cache.end()){
           
            offset = CSchemaManager::GetSchemaOffset(lib,clss.c_str(),field.c_str()).value();
            if(offset){
                cache.emplace(std::pair<std::string, uintptr_t>(clss.append(field), offset));
                return offset;
            }
               
            return 0;
        }
        else{
            return cache.at(clss.append(field));
        }

        return 0;
    }*/
    uintptr_t get(std::string clss, std::string field, bool dump = false);
    void init(bool dump);
    int calls;
private:
    std::unordered_map<std::string, uintptr_t> cache;
};