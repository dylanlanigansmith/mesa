#pragma once

#include "memory.hpp"
#include "../frame.hpp"
#include "../../SignatureScanner/Include/SignatureScanner.hpp"


using namespace SignatureScanner;
class sigs{
    public:
        const char *trace_sig = "E8 ? ? ? ? 48 23 9D ? ? ? ?"; //todo: #def these
        const char *local_sig = "48 83 3D ? ? ? ? ? 0F 95 C0 C3";

        const char *ent_sig = "74 61 48 8B 15 ? ? ? ?";

        const char *matrix_sig = "4C 8D 05 ? ? ? ? 48 8B 38 48 8D 0D ? ? ? ?";

        const char *C4_sig = "48 89 5D E8 48 8D 3D ?? ?? ?? ??";
        const char *enginetraceptr_sig = "48 8D 05 ? ? ? ? 31 F6 4C 89 E7 4C 8B 38";

        const char *setmodel_sig = "E8 ? ? ? ? 49 8B 3F F3 41 0F 10 47 ?";

        const char *weaponc4_sig = "48 8D 15 ? ? ? ? 8B 02 83 E8 01";

        const char *vmt_sig = "E8 ? ? ? ? 31 D2 31 F6 4C 89 EF E8 ? ? ? ? 4C 89 E2";

        const char *mesh_sig = "55 48 89 E5 48 83 EC 10 48 89 75 F8 48 39 B7 ? ? ? ?";
        sigs(){
           
        }
        ~sigs(){

        }
        uintptr_t getLocalPlayerOffset()
        {
            auto& cl =  kf->GetMem()->GetModule(CLIENT_LIBX);
            if(!cl)
                return 0;
            if(cl->start <= 0)
                return 0;
           
            uintptr_t base = cl->base; //just .text ig

            uintptr_t size = cl->size;
            uintptr_t end = cl->base + cl->size;
            auto signature = ByteSignature(local_sig);
          
          
            unsigned char* bytes = (unsigned char*)(base);
            void* hit = signature.findNext<void*>(bytes, base + size);
            std::vector<void*> hits = signature.findAll<void*>(bytes, base + size);
            auto offset = reinterpret_cast<uintptr_t>(hit); //* idk
            if(offset == 0x0){
                kf->Log(xs("Couldn't Find LocalPlayer"));  return 0;
            }

            uint32_t abs = offset - cl->start;
            unsigned char* bytes_at_offset = (unsigned char*)(offset);
            char buffer [60];
                buffer[16] = 0;
            for(int j = 0; j < 8; j++)
                sprintf(&buffer[2*j], "%02X", bytes_at_offset[j]);

            int32_t rel = 0;
            uint32_t final = 0;
            if(bytes_at_offset[0] == 0x48){
                if(bytes_at_offset[1] == 0x83){
                   /*rel = (bytes_at_offset[5] << 24) |
                         (bytes_at_offset[4] << 16) |
                         (bytes_at_offset[3] << 8) |
                          bytes_at_offset[2];*/
                    rel = *(int32_t*)(bytes_at_offset + 3);
                } //48 83 3D A0 0A B2 02 00 -> 00 02 B2 0A A0 
            }
            
            if(rel != 0){
                final = abs + rel + 0x8;
                //kf->Log(kf->ToHex(rel) + " rel");
               final = roundToLikelyAddress(final);
                kf->Log(xs(" Found Local Player @") + kf->ToHex(final), LOG_INFO);
                return final;
            }
            kf->Log(xs("Couldn't Find LocalPlayer. Printing Debug info:"));
           
            kf->Log(hits.size(), "#hits ");
            kf->Log(base, " .text Base");
            kf->Log(cl->start, "Module Start ");
            
            kf->Log(hits.size(), xs("#hits "));
            kf->Log(base, xs(" .text Base"));
            kf->Log(cl->start, xs("Module Start "));
            char buf[64];
            sprintf(buf, "%lx", offset);
            kf->Log("Offset (trust): " + std::string(buf));
            kf->Log(offset - cl-> start, xs("Offset from modstart: "));
            kf->Log(kf->ToHex(abs) + xs(" abs"));
            kf->Log(kf->ToHex(final) + xs(" final"));
            kf->Log(xs("Found Bytes") + std::string(buffer));
            return 0;     
            return 0;     
        } //end getLP
       uintptr_t getEntityListOffset()
       {
             auto& cl =  kf->GetMem()->GetModule(CLIENT_LIBX);
            if(!cl)
                return 0;
            if(cl->start <= 0)
                return 0;
           
            uintptr_t base = cl->base; //just .text ig

            uintptr_t size = cl->size;
            uintptr_t end = cl->base + cl->size;
            auto signature = ByteSignature(ent_sig);
          
          
            unsigned char* bytes = (unsigned char*)(base);
            void* hit = signature.findNext<void*>(bytes, base + size);
            std::vector<void*> hits = signature.findAll<void*>(bytes, base + size);
            auto offset = reinterpret_cast<uintptr_t>(hit); //* idk
            if(offset == 0x0 || hits.size() == 0){
                kf->Log(xs("Couldn't Find Entlist"));  return 0;
            }

            uint32_t abs = offset - cl->start;
            unsigned char* bytes_at_offset = (unsigned char*)(offset);
            char buffer [60];
                buffer[16] = 0;
            for(int j = 0; j < 8; j++)
                sprintf(&buffer[2*j], "%02X", bytes_at_offset[j]);
            /*
            .text:0000000000BC652D 74 61                                   jz      short loc_BC6590
            .text:0000000000BC652F 48 8B 15 32 AF 95 02                    mov     rdx, cs:EntList //
            .text:0000000000BC6536 44 89 F0                                mov     eax, r14d
            .text:0000000000BC6539 C1 F8 09       
            */
            int32_t rel = 0;
            uint32_t final = 0;
            if(bytes_at_offset[0] == 0x74 && bytes_at_offset[1] == 0x61 && bytes_at_offset[2] == 0x48) {
                if(bytes_at_offset[3] == 0x8B && bytes_at_offset[4] == 0x15){
                    rel = *(int32_t*)(bytes_at_offset + 5);
                } //74 61 48 8B 15 32 AF 95 02 
            }
         
            
            if(rel != 0){
                final = abs + rel + 0x8 ;//+ 0x1; //HARDCODE should be closest "8" 
                //like rn it gives 0x123456f37 and we want 0x123456f38
             //    kf->Log(kf->ToHex(final) + " pre round final");
                final = roundToLikelyAddress(final);
              //  kf->Log(kf->ToHex(rel) + " rel");
               
                kf->Log(xs(" Found EntList @") + kf->ToHex(final), LOG_INFO);
                return final;
            }
            kf->Log(xs("Couldn't Find EntList. Printing Debug info:"));
           
           kf->Log(hits.size(), xs("#hits "));
            kf->Log(base, xs(" .text Base"));
            kf->Log(cl->start, xs("Module Start "));
            char buf[64];
            sprintf(buf, "%lx", offset);
            kf->Log("Offset (trust): " + std::string(buf));
            kf->Log(offset - cl-> start, xs("Offset from modstart: "));
            kf->Log(kf->ToHex(abs) + xs(" abs"));
            kf->Log(kf->ToHex(final) + xs(" final"));
            kf->Log(xs("Found Bytes") + std::string(buffer));
            return 0;     
       }//END ENT 

       
       uintptr_t getViewMatrixOffset()
        {
             auto& cl =  kf->GetMem()->GetModule(CLIENT_LIBX);
            if(!cl)
                return 0;
            if(cl->start <= 0)
                return 0;
           
            uintptr_t base = cl->base; //just .text ig

            uintptr_t size = cl->size;
            uintptr_t end = cl->base + cl->size;
            auto signature = ByteSignature(matrix_sig);
          
          
            unsigned char* bytes = (unsigned char*)(base);
            void* hit = signature.findNext<void*>(bytes, base + size);
            std::vector<void*> hits = signature.findAll<void*>(bytes, base + size);
            auto offset = reinterpret_cast<uintptr_t>(hit); //* idk
            if(offset == 0x0 || hits.size() == 0){
                kf->Log(xs("Couldn't Find ViewMatrix"));  return 0;
            }

            uint32_t abs = offset - cl->start;
            unsigned char* bytes_at_offset = (unsigned char*)(offset);
            char buffer [60];
                buffer[16] = 0;
            for(int j = 0; j < 8; j++)
                sprintf(&buffer[2*j], "%02X", bytes_at_offset[j]);
            /*
           .text:0000000000DE0A39 4C 8D 05 60 EC 93 02                    lea     r8, ViewMatrix
            */
            int32_t rel = 0;
            uint32_t final = 0;
            if(bytes_at_offset[0] == 0x4C && bytes_at_offset[1] == 0x8D && bytes_at_offset[2] == 0x05) {
                
                    rel = *(int32_t*)(bytes_at_offset + 3);
                //4C 8D 05 60 EC 93 02 
            }
         
            
            if(rel != 0){
                final = abs + rel + 0x8 ;// - 0x2; //HARDCODE should be closest "8" 
                //like rn it gives 0x123456f37 and we want 0x123456f38 -> need function to find most likely address to b aligned to 
            //     kf->Log(kf->ToHex(final) + " pre round final");
                 final = roundToLikelyAddress(final);
               // kf->Log(kf->ToHex(rel) + " rel");
               
                kf->Log(xs(" Found ViewMatrix @") + kf->ToHex(final), LOG_INFO);
                return final;
            }
            kf->Log(xs("Couldn't Find ViewMatrix. Printing Debug info:"));
           
            kf->Log(hits.size(), "#hits ");
            kf->Log(base, " .text Base");
            kf->Log(cl->start, "Module Start ");
            
            
           
            kf->Log(hits.size(), xs("#hits "));
            kf->Log(base, xs(" .text Base"));
            kf->Log(cl->start, xs("Module Start "));
            char buf[64];
            sprintf(buf, "%lx", offset);
            kf->Log(xs("Offset (trust): ") + std::string(buf));
            kf->Log(offset - cl-> start, xs("Offset from modstart: "));
            kf->Log(kf->ToHex(abs) + xs(" abs"));
            kf->Log(kf->ToHex(final) + xs(" final"));
            kf->Log(xs("Found Bytes") + std::string(buffer));
            return 0;     
       } //END MATRIX

        uintptr_t getPlantedC4Offset()
        {
             auto& cl =  kf->GetMem()->GetModule(CLIENT_LIBX);
            if(!cl)
                return 0;
            if(cl->start <= 0)
                return 0;
           
            uintptr_t base = cl->base; //just .text ig

            uintptr_t size = cl->size;
            uintptr_t end = cl->base + cl->size;
            auto signature = ByteSignature(C4_sig);
          
          
            unsigned char* bytes = (unsigned char*)(base);
            void* hit = signature.findNext<void*>(bytes, base + size);
            std::vector<void*> hits = signature.findAll<void*>(bytes, base + size);
            auto offset = reinterpret_cast<uintptr_t>(hit); //* idk
            if(offset == 0x0 || hits.size() == 0){
                kf->Log(xs("Couldn't Find PlantedC4"));  return 0;
            }

            uint32_t abs = offset - cl->start;
            
            /*
           .text:0000000000E934DF 48 89 5D E8                             mov     [rbp+var_18], rbx
            .         text:0000000000E934E3 48 8D 3D 56 15 8A 02                    lea     rdi, PlantedC4
            */
           //unsigned char signature[] = { 0x48, 0x89, 0x87 };
           //if (memcmp(bytes_at_offset, signature, sizeof(signature)) == 0) { //THE RIGHT WAY
            int32_t rel = 0;
            uint32_t final = 0;
             unsigned char* bytes_at_offset = (unsigned char*)(offset);
            if(bytes_at_offset[0] == 0x48 && bytes_at_offset[1] == 0x89 && bytes_at_offset[2] == 0x5D) { //
                bytes_at_offset = (unsigned char*)(offset + 0x4);
                 if(bytes_at_offset[0] == 0x48 && bytes_at_offset[1] == 0x8D && bytes_at_offset[2] == 0x3D) {
                    rel = *(int32_t*)(bytes_at_offset + 3);

                 }
                    
                //48 89 5D E8 48 8D 3D ?? ?? ?? ??
            }
           
          
            
            if(rel != 0){
                final = abs + rel + 0x8 ;
               //  kf->Log(kf->ToHex(final) + " pre round final");
                 final = roundToLikelyAddress(final, 16); //THIS BYTE ALIGNMENT MIGHT BE FOR EVERYTHING
               // kf->Log(kf->ToHex(rel) + " rel");
               
                kf->Log(xs(" Found PlantedC4 @") + kf->ToHex(final), LOG_INFO);
                return final;
            }

            char buffer [60];
                buffer[16] = 0;
            for(int j = 0; j < 8; j++)
                sprintf(&buffer[2*j], "%02X", bytes_at_offset[j]);
            kf->Log(xs("Couldn't Find PlantedC4. Printing Debug info:"));
           
             kf->Log(hits.size(), xs("#hits "));
            kf->Log(base, xs(" .text Base"));
            kf->Log(cl->start, xs("Module Start "));
            char buf[64];
            sprintf(buf, "%lx", offset);
            kf->Log(xs("Offset (trust): ") + std::string(buf));
            kf->Log(offset - cl-> start, xs("Offset from modstart: "));
            kf->Log(kf->ToHex(abs) + xs(" abs"));
            kf->Log(kf->ToHex(final) + xs(" final"));
            kf->Log(xs("Found Bytes") + std::string(buffer));
            return 0;        
       } //end Planted C4

       uintptr_t getTraceShapeOffset()
        {
             auto& cl =  kf->GetMem()->GetModule(CLIENT_LIBX);
            if(!cl)
                return 0;
            if(cl->start <= 0)
                return 0;
           
            uintptr_t base = cl->base; //just .text ig

            uintptr_t size = cl->size;
            uintptr_t end = cl->base + cl->size;
            auto signature = ByteSignature(trace_sig);
          
          
            unsigned char* bytes = (unsigned char*)(base);
            void* hit = signature.findNext<void*>(bytes, base + size);
            std::vector<void*> hits = signature.findAll<void*>(bytes, base + size);
            auto offset = reinterpret_cast<uintptr_t>(hit); //* idk
            if(offset == 0x0 || hits.size() == 0){
                kf->Log(xs("Couldn't Find TraceShape"));  return 0;
            }

            uint32_t abs = offset - cl->start;
            
            /*
                .text:0000000000C2DDE9 E8 F2 F7 FA FF                          call    TraceShape
                .text:0000000000C2DDEE 48 23 9D 38 FF FF FF                    and     rbx, [rbp+var_C8]
            */
         
            int32_t rel = 0;
            uint32_t final = 0;
             unsigned char* bytes_at_offset = (unsigned char*)(offset);
             if (bytes_at_offset[0] == 0xE8)
             {
                    //E8 F2 F7 FA FF     
                rel = *(int32_t *)(bytes_at_offset + 1);
             }

            if(rel != 0){
                final = abs + rel + 0x8 ;
                // kf->Log(kf->ToHex(final) + " pre round final");
                 final = roundToLikelyAddress(final, 16); //THIS BYTE ALIGNMENT MIGHT BE FOR EVERYTHING
            //    kf->Log(kf->ToHex(rel) + " rel");
               
                kf->Log(xs(" Found TraceShape @") + kf->ToHex(final), LOG_INFO);
                return final;
            }

            char buffer [60];
                buffer[16] = 0;
            for(int j = 0; j < 8; j++)
                sprintf(&buffer[2*j], "%02X", bytes_at_offset[j]);
            kf->Log(xs("Couldn't Find TraceShape. Printing Debug info:"));
           
            kf->Log(hits.size(), xs("#hits "));
            kf->Log(base, xs(" .text Base"));
            kf->Log(cl->start, xs("Module Start "));
            char buf[64];
            sprintf(buf, "%lx", offset);
            kf->Log(xs("Offset (trust): ") + std::string(buf));
            kf->Log(offset - cl-> start, xs("Offset from modstart: "));
            kf->Log(kf->ToHex(abs) + xs(" abs"));
            kf->Log(kf->ToHex(final) + xs(" final"));
            kf->Log(xs("Found Bytes") + std::string(buffer));
            return 0;        
       } //end TraceShape

        uintptr_t getEngineTraceOffset()
        {
             auto& cl =  kf->GetMem()->GetModule(CLIENT_LIBX);
            if(!cl)
                return 0;
            if(cl->start <= 0)
                return 0;
           
            uintptr_t base = cl->base; //just .text ig

            uintptr_t size = cl->size;
            uintptr_t end = cl->base + cl->size;
            auto signature = ByteSignature(enginetraceptr_sig);
          
          
            unsigned char* bytes = (unsigned char*)(base);
            void* hit = signature.findNext<void*>(bytes, base + size);
            std::vector<void*> hits = signature.findAll<void*>(bytes, base + size);
            auto offset = reinterpret_cast<uintptr_t>(hit); //* idk
            if(offset == 0x0 || hits.size() == 0){
                kf->Log(xs("Couldn't Find EngineTrace"));  return 0;
            }

            uint32_t abs = offset - cl->start;
            
            /*
               .text:0000000000C2DCE4 48 8D 05 45 51 8A 02                    lea     rax, EngineTracePtr
            */
         
            int32_t rel = 0;
            uint32_t final = 0;
             unsigned char* bytes_at_offset = (unsigned char*)(offset);
             if (bytes_at_offset[0] == 0x48 && bytes_at_offset[1] == 0x8D && bytes_at_offset[2] == 0x05)
             {
                    //48 8D 05 45 51 8A 02 
                rel = *(int32_t *)(bytes_at_offset + 3);
             }

            if(rel != 0){
                final = abs + rel + 0x8 ;
               // kf->Log(kf->ToHex(final) + " pre round final");
                 final = roundToLikelyAddress(final, 16); //THIS BYTE ALIGNMENT MIGHT BE FOR EVERYTHING
               // kf->Log(kf->ToHex(rel) + " rel");
               
                kf->Log(xs(" Found EngineTrace @") + kf->ToHex(final), LOG_INFO);
                return final;
            }

            char buffer [60];
                buffer[16] = 0;
            for(int j = 0; j < 8; j++)
                sprintf(&buffer[2*j], "%02X", bytes_at_offset[j]);
            kf->Log(xs("Couldn't Find EngineTrace. Printing Debug info:"));
           
            kf->Log(hits.size(), xs("#hits "));
            kf->Log(base, xs(" .text Base"));
            kf->Log(cl->start, xs("Module Start "));
            char buf[64];
            sprintf(buf, "%lx", offset);
            kf->Log(xs("Offset (trust): ") + std::string(buf));
            kf->Log(offset - cl-> start, xs("Offset from modstart: "));
            kf->Log(kf->ToHex(abs) + xs(" abs"));
            kf->Log(kf->ToHex(final) + xs(" final"));
            kf->Log(xs("Found Bytes") + std::string(buffer));
            return 0;     
       }
        uintptr_t getSetModelOffset(){
            auto& cl =  kf->GetMem()->GetModule(CLIENT_LIBX);
            if(!cl)
                return 0;
            if(cl->start <= 0)
                return 0;
           
            uintptr_t base = cl->base; //just .text ig

            uintptr_t size = cl->size;
            uintptr_t end = cl->base + cl->size;
            auto signature = ByteSignature(setmodel_sig);
          
          
            unsigned char* bytes = (unsigned char*)(base);
            void* hit = signature.findNext<void*>(bytes, base + size);
            std::vector<void*> hits = signature.findAll<void*>(bytes, base + size);
            auto offset = reinterpret_cast<uintptr_t>(hit); //* idk
            if(offset == 0x0 || hits.size() == 0){
                kf->Log(xs("Couldn't Find SetModel"));  return 0;
            }

            uint32_t abs = offset - cl->start;
            
            /*
               E8 ? ? ? ? 49 8B 3F F3 41 0F 10 47 ? 
                .text:0000000000F867BB 49 0F 44 F4                             cmovz   rsi, r12
                .text:0000000000F867BF E8 3C BB BC FF                          call    SetModel
            */
         
            int32_t rel = 0;
            uint32_t final = 0;
             unsigned char* bytes_at_offset = (unsigned char*)(offset);
              if (bytes_at_offset[0] == 0xE8)
             {
                    //E8 E8 3C BB BC FF 
                rel = *(int32_t *)(bytes_at_offset + 1);
             }

            if(rel != 0){
                final = abs + rel + 0x8 ;
               // kf->Log(kf->ToHex(final) + " pre round final");
                 final = roundToLikelyAddress(final, 16); //THIS BYTE ALIGNMENT MIGHT BE FOR EVERYTHING
               // kf->Log(kf->ToHex(rel) + " rel");
               
                kf->Log(xs(" Found SetModel @") + kf->ToHex(final), LOG_INFO);
                return final;
            }

            char buffer [60];
                buffer[16] = 0;
            for(int j = 0; j < 8; j++)
                sprintf(&buffer[2*j], "%02X", bytes_at_offset[j]);
            kf->Log(xs("Couldn't Find SetModel Printing Debug info:"));
           
            kf->Log(hits.size(), xs("#hits "));
            kf->Log(base, xs(" .text Base"));
            kf->Log(cl->start, xs("Module Start "));
            char buf[64];
            sprintf(buf, "%lx", offset);
            kf->Log(xs("Offset (trust): ") + std::string(buf));
            kf->Log(offset - cl-> start, xs("Offset from modstart: "));
            kf->Log(kf->ToHex(abs) + xs(" abs"));
            kf->Log(kf->ToHex(final) + xs(" final"));
            kf->Log(xs("Found Bytes") + std::string(buffer));
            return 0;     

        } //end setmodel
         uintptr_t getWeaponC4Offset(){
            auto& cl =  kf->GetMem()->GetModule(CLIENT_LIBX);
            if(!cl)
                return 0;
            if(cl->start <= 0)
                return 0;
           
            uintptr_t base = cl->base; //just .text ig

            uintptr_t size = cl->size;
            uintptr_t end = cl->base + cl->size;
            auto signature = ByteSignature(weaponc4_sig);
          
          
            unsigned char* bytes = (unsigned char*)(base);
            void* hit = signature.findNext<void*>(bytes, base + size);
            std::vector<void*> hits = signature.findAll<void*>(bytes, base + size);
            auto offset = reinterpret_cast<uintptr_t>(hit); //* idk
            if(offset == 0x0 || hits.size() == 0){
                kf->Log(xs("Couldn't Find WeaponC4"));  return 0;
            }

            uint32_t abs = offset - cl->start;
            
            /*
             //48 8D 15 ? ? ? ? 8B 02 83 E8 01  
                .text:0000000000EB9870 48 8D 15 59 DE 69 02                    lea     rdx, WeaponC4
            */
         
            int32_t rel = 0;
            uint32_t final = 0;
             unsigned char* bytes_at_offset = (unsigned char*)(offset);
             
                 if(bytes_at_offset[0] == 0x48 && bytes_at_offset[1] == 0x8D && bytes_at_offset[2] == 0x15) {
                    rel = *(int32_t*)(bytes_at_offset + 3);

                 }
                    
                //48 8D 15 59 DE 69 02      
            

            if(rel != 0){
                final = abs + rel + 0x8 ;
               // kf->Log(kf->ToHex(final) + " pre round final");
                 final = roundToLikelyAddress(final, 16); //THIS BYTE ALIGNMENT MIGHT BE FOR EVERYTHING
               // kf->Log(kf->ToHex(rel) + " rel");
               
                kf->Log(xs(" Found WeaponC4 @") + kf->ToHex(final), LOG_INFO);
                return final;
            }

            char buffer [60];
                buffer[16] = 0;
            for(int j = 0; j < 8; j++)
                sprintf(&buffer[2*j], "%02X", bytes_at_offset[j]);
            kf->Log(xs("Couldn't Find WeaponC4 Printing Debug info:"));
           
            kf->Log(hits.size(), xs("#hits "));
            kf->Log(base, xs(" .text Base"));
            kf->Log(cl->start, xs("Module Start "));
            char buf[64];
            sprintf(buf, "%lx", offset);
            kf->Log(xs("Offset (trust): ") + std::string(buf));
            kf->Log(offset - cl-> start, xs("Offset from modstart: "));
            kf->Log(kf->ToHex(abs) + xs(" abs"));
            kf->Log(kf->ToHex(final) + xs(" final"));
            kf->Log(xs("Found Bytes") + std::string(buffer));
            return 0;     

        }
         uintptr_t getVMTOffset(){
            auto& cl =  kf->GetMem()->GetModule(CLIENT_LIBX);
            if(!cl)
                return 0;
            if(cl->start <= 0)
                return 0;
           
            uintptr_t base = cl->base; //just .text ig

            uintptr_t size = cl->size;
            uintptr_t end = cl->base + cl->size;
            auto signature = ByteSignature(vmt_sig);
          
          
            unsigned char* bytes = (unsigned char*)(base);
            void* hit = signature.findNext<void*>(bytes, base + size);
            std::vector<void*> hits = signature.findAll<void*>(bytes, base + size);
            auto offset = reinterpret_cast<uintptr_t>(hit); //* idk
            if(offset == 0x0 || hits.size() == 0){
                kf->Log(xs("Couldn't Find VMTCheck"));  return 0;
            }

            uint32_t abs = offset - cl->start;
            
            /*
             //E8 ? ? ? ? 31 D2 31 F6 4C 89 EF E8 ? ? ? ? 4C 89 E2
                .text:0000000000652B7F E8 FC E5 FF FF                          call    sub_651180
            */
         
            int32_t rel = 0;
            uint32_t final = 0;
             unsigned char* bytes_at_offset = (unsigned char*)(offset);
             
                 if(bytes_at_offset[0] == 0xE8) {
                    rel = *(int32_t*)(bytes_at_offset + 1);

                 }
                    
                //48 8D 15 59 DE 69 02      
            

            if(rel != 0){
                final = abs + rel + 0x8 ;
               // kf->Log(kf->ToHex(final) + " pre round final");
                 final = roundToLikelyAddress(final, 16); //THIS BYTE ALIGNMENT MIGHT BE FOR EVERYTHING
               // kf->Log(kf->ToHex(rel) + " rel");
               
                kf->Log(xs(" Found VMTCheck @") + kf->ToHex(final), LOG_INFO);
               return final;
            }

            char buffer [60];
                buffer[16] = 0;
            for(int j = 0; j < 8; j++)
                sprintf(&buffer[2*j], "%02X", bytes_at_offset[j]);
            kf->Log(xs("Couldn't Find VMTCheck Printing Debug info:"));
           
            kf->Log(hits.size(), xs("#hits "));
            kf->Log(base, xs(" .text Base"));
            kf->Log(cl->start, xs("Module Start "));
            char buf[64];
            sprintf(buf, "%lx", offset);
            kf->Log(xs("Offset (trust): ") + std::string(buf));
            kf->Log(offset - cl-> start, xs("Offset from modstart: "));
            kf->Log(kf->ToHex(abs) + xs(" abs"));
            kf->Log(kf->ToHex(final) + xs(" final"));
            kf->Log(xs("Found Bytes") + std::string(buffer));
            return 0;     

        }
         uintptr_t getSetMeshOffset(){
            auto& cl =  kf->GetMem()->GetModule(CLIENT_LIBX);
            if(!cl)
                return 0;
            if(cl->start <= 0)
                return 0;
           
            uintptr_t base = cl->base; //just .text ig

            uintptr_t size = cl->size;
            uintptr_t end = cl->base + cl->size;
            auto signature = ByteSignature(mesh_sig);
          
          
            unsigned char* bytes = (unsigned char*)(base);
            void* hit = signature.findNext<void*>(bytes, base + size);
            std::vector<void*> hits = signature.findAll<void*>(bytes, base + size);
            auto offset = reinterpret_cast<uintptr_t>(hit); //* idk
            if(offset == 0x0 || hits.size() == 0){
                kf->Log(xs("Couldn't Find SetMesh"));  return 0;
            }

            uint32_t abs = offset - cl->start;
            
            /*
             //55 48 89 E5 48 83 EC 10 48 89 75 F8 48 39 B7 ? ? ? ? 
                .text:0000000000C6B810                         SetMeshish      proc near               ; CODE XREF: NotSet_Mesh_Group_Mask+79↑j
                .text:0000000000C6B810
                .text:0000000000C6B810                         var_8           = qword ptr -8
                .text:0000000000C6B810
                .text:0000000000C6B810 55                                      push    rbp
                .text:0000000000C6B811 48 89 E5                                mov     rbp, rsp
                .text:0000000000C6B814 48 83 EC 10                             sub     rsp, 10h
                .text:0000000000C6B818 48 89 75 F8                             mov     [rbp+var_8], rsi
                .text:0000000000C6B81C 48 39 B7 E0 02 00 00                    cmp     [rdi+2E0h], rsi
            */
         
            int32_t rel = 0;
            uint32_t final = 0;
             unsigned char* bytes_at_offset = (unsigned char*)(offset);
             
               
                //48 8D 15 59 DE 69 02      
            
            
            if(abs != 0){
                 kf->Log(xs(" Found SetMesh @") + kf->ToHex(final), LOG_INFO);
              
                 final = roundToLikelyAddress(abs, 16); //THIS BYTE ALIGNMENT MIGHT BE FOR EVERYTHING
               // kf->Log(kf->ToHex(rel) + " rel");
               
               
                return final;
            }

            char buffer [60];
                buffer[16] = 0;
            for(int j = 0; j < 8; j++)
                sprintf(&buffer[2*j], "%02X", bytes_at_offset[j]);
            kf->Log(xs("Couldn't Find SetMesh Printing Debug info:"));
           
            kf->Log(hits.size(), xs("#hits "));
            kf->Log(base, xs(" .text Base"));
            kf->Log(cl->start, xs("Module Start "));
            char buf[64];
            sprintf(buf, "%lx", offset);
            kf->Log(xs("Offset (trust): ") + std::string(buf));
            kf->Log(offset - cl-> start, xs("Offset from modstart: "));
            kf->Log(kf->ToHex(abs) + xs(" abs"));
            kf->Log(kf->ToHex(final) + xs(" final"));
            kf->Log(xs("Found Bytes") + std::string(buffer));
            return 0;     

        }
    private:
        uint64_t roundToLikelyAddress(uint64_t relativeAddress, uint64_t alignment = 0x8)
        {
            // Calculate the remainder when dividing by alignment
            uint64_t remainder = relativeAddress % alignment;

            // Round up or down based on the remainder
            if (remainder < (alignment / 2))
            {
                // Round down
                return relativeAddress - remainder;
            }
            else
            {
                // Round up
                return relativeAddress + (alignment - remainder);
            }
        }
};