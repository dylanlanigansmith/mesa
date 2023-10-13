#include "memory.hpp"

#include <stdio.h>
#include <string.h>
 #include <dlfcn.h>
 #include <link.h>

 #include "../globals.hpp"
Module::Module(const char* fn){
    
    name = fn;
    base = size = start = 0;
    handle = nullptr;
}
Module::~Module(){
   // if(!handle)
      //  return;
}
bool Module::Get(){
    if(handle)
        return true;
    handle = dlopen(name.c_str(), RTLD_LAZY | RTLD_NOLOAD);
    InitBounds();
    
    loaded = true;
    return handle != nullptr;
}
void Module::InitBounds(){
    if(!handle)
        return;
    dl_iterate_phdr(
        [](struct dl_phdr_info *info, size_t, void *data)
        {
            Module *currentModule = static_cast<Module *>(data);
            if (strstr(info->dlpi_name, currentModule->name.c_str()) != 0)
            {
                currentModule->start = info->dlpi_addr;
                for (Elf64_Half i = 0; i < info->dlpi_phnum; ++i)
                {
                    const auto &phdr = info->dlpi_phdr[i];
                    
                    if (phdr.p_type != PT_LOAD || !(phdr.p_flags & PF_X))
                        continue;

                    currentModule->base = info->dlpi_addr + phdr.p_vaddr;
                    currentModule->size = phdr.p_memsz;
                    return 1;
                }
            }

            return 0;
        },
        static_cast<void *>(this));
}
uintptr_t Module::GetProcAddr(const char* procName){
    uintptr_t ret = 0;
    if(handle){
        ret = reinterpret_cast<uintptr_t>(dlsym(handle, procName));
    }
    return ret;
}
Memory::Memory()
{
    isReady = false;

}
Memory::~Memory()
{

}
bool Memory::TargetReady(const char *target)
{
    return true;
}





bool Memory::Ready(){
    return isReady;
}

moduleptr_t&  Memory::GetModule(const char* n){
    return GetModulePtr(n);
}
void Memory::InitThread()
{

    while(!GetModule(NAVSYSTEM_LIBX)){
         std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
    GetModule(CLIENT_LIBX);
    GetModule(SCHEMA_LIBX);
    isReady = true;
    return;
}

moduleptr_t& Memory::GetModulePtr(const char* n){
    auto hash = fnv1a::Hash(n);
    auto it = modulecache.find(hash);
    if(it != modulecache.end()){
        return it->second;
    }
    auto mod = std::make_unique<Module>(n);
    if(mod->Get()){
        return modulecache.emplace(hash, std::move(mod)).first->second;
    }
    //log not found
    static moduleptr_t null{};
    return null;
}
bool Memory::Initialize()
{
    init_t = std::thread(&Memory::InitThread, this); //sus
    init_t.detach();
    return true;
}

std::string Memory::exec(const char* cmd){
   std::array<char, 128> buffer;
   std::string result;
   std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
   if (!pipe)
   {
      throw std::runtime_error("popen() failed!");
   }
   while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
   {
      result += buffer.data();
   }
   return result;
}



uintptr_t Module::GetInterface(const char* version){
     uintptr_t rv = 0;
    if (handle) {
        CPointer pCreateInterface = GetProcAddr("CreateInterface");
        if (!pCreateInterface.IsValid()) {
            return rv;
        }

        // Used internally to register classes.
        struct InterfaceReg {
            std::add_pointer_t<uintptr_t()> m_CreateFn;
            const char* m_pName;
            InterfaceReg* m_pNext;  // For the global list.
        };

        InterfaceReg* s_pInterfaceRegs = 
        pCreateInterface.Absolute(1, 0).Absolute(19, 0).Dereference(1).Get<InterfaceReg*>();

        uint32_t versionHash = fnv1a::Hash(version);
        for (; s_pInterfaceRegs; s_pInterfaceRegs = s_pInterfaceRegs->m_pNext) {
            if (fnv1a::Hash(s_pInterfaceRegs->m_pName) == versionHash) {
                rv = s_pInterfaceRegs->m_CreateFn();
                break;
            }
        }
    }

    return rv;
}



CPointer Memory::GetInterfacePtr(const char* libName, const char* version){
   CPointer rv = 0;

    auto& library = GetModulePtr(libName);
    if (!library) {
        //LOG("[interface] Couldn't retrieve {} because {} was not loaded", version, libName);
        return rv;
    }

    rv = library->GetInterface(version);
   //LOG Log("[interface] {} -> {} -> {}", libName, version, rv.Get<void*>());

    return rv;
}
CPointer Memory::GetInterface(const char* libName, const char* version){
    return GetInterfacePtr(libName, version);
}