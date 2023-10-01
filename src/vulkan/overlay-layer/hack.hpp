#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>

#include <sys/stat.h>
#include <dirent.h>
#include <libgen.h>
#include <signal.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <sys/uio.h>
#include <string>
#include <sstream>
#include <vector>

struct MapModuleMemoryRegion;



#define FINDPATTERN_CHUNKSIZE 0x1000
struct MapModuleMemoryRegion
{
public:
    // Memory
    unsigned long start;
    unsigned long end;

    // Permissions
    bool readable;
    bool writable;
    bool executable;
    bool shared;

    // File data
    unsigned long offset;
    unsigned char deviceMajor;
    unsigned char deviceMinor;
    unsigned long inodeFileNumber;
    std::string pathname;
    std::string filename;

    unsigned long client_start;

    
};
