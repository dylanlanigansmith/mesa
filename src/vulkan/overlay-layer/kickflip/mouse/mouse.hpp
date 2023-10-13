#pragma once
#include <linux/uinput.h>

#include "../xorstr.hpp"

#define UINPUT xs("/dev/uinput").c_str()

class mousetrap{
    public:
        mousetrap();
        ~mousetrap();
        std::string err;
        bool isReady() { return ready;}
        bool setmouse(int x, int y);
    private:
        bool ready;
        int fd;
        struct uinput_user_dev uidev;
};