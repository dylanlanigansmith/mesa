#pragma once

#include <string>
#include <signal.h>
#include <vector>
#include <iomanip>
#include <stdio.h>
#include <unistd.h> 
#include <cstdio>
#include <memory>
#include <iostream>
#include <array>
#include <thread>
#include <chrono>
#include <unordered_map>
#include <functional>
#include "pointer.hpp"
class Module{
    public:
        Module(const char* fn );
        ~Module();
        bool Get();
        std::string name;
        uint64_t base; //exec
        uint64_t start; //start
        uint64_t size;
        uint32_t hash;
        uintptr_t GetProcAddr(const char* procName);
        uintptr_t GetInterface(const char* version);
    private:
        bool loaded;
        void* handle;
        void InitBounds();

};
using moduleptr_t = std::unique_ptr<Module> ;


class Memory{
public:
    Memory();
    ~Memory();
    bool TargetReady(const char* target);
    bool Initialize();
    std::string exec(const char *cmd);
    bool Ready();
    moduleptr_t&  GetModule(const char* n);
    CPointer GetInterface(const char* libName, const char* version);
private:
    bool isReady;
    std::thread init_t;
    void InitThread();
    std::unordered_map<uint32_t, moduleptr_t> modulecache;
    moduleptr_t& GetModulePtr(const char* n);
    CPointer GetInterfacePtr(const char* libName, const char* version);
};

namespace fnv1a {
    constexpr uint32_t Hash(const char* str) noexcept { return (*str ? (Hash(str + 1) ^ *str) * 0x01000193 : 0x811c9dc5); }
}  // namespace fnv1a