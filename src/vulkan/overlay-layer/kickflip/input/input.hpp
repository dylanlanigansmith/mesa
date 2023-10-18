#pragma once

#include <X11/Xlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <X11/Xlib.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "kickflip/s2_types.hpp"
#include <poll.h>
#include <signal.h>
#include <vector>
#include <iomanip>
#include <stdio.h>
#include <malloc.h>
#include <thread>
#include <filesystem>
#include <string>
#include "imgui.h"
static int _XlibErrorHandler(Display *display, XErrorEvent *event)
{
   //fprintf(stderr, "An error occured detecting the mouse position\n");
   return True;
}
enum Keys
{
   MOUSE1 = 0,
   MOUSE2,
   SHIFT,
   TAB,
   TILDE,
   LALT,
   MOUSE4,
   MOUSE5,
};
struct input
{
   uint8_t keys[6];
   Vector2 mouse;
   std::string error;
   bool stopInput = false;
};

class InputManager {
    public:
        InputManager();
        ~InputManager();
        bool hasInitialized();
        int GetNumPolls();
        bool Mouse(int n);
        ImVec2 MousePos();
        bool MenuKey();
        bool ShouldOpenMenu();
        void Shutdown();
        input in;
    private:
        void GetInput();
        void NewGetInput();

       // void ZeroKeys(); //fuck that
        bool supports_key_events(const int &fd);
        int fallback_mb;
        int fallback_kb;
        int event_mb;
        int event_kb;
        int found_mb;
        int found_kb;
    private:
        void getEventNumbers();
        std::string getEventName(int fd);
        std::vector<pollfd> poll_fds;
        bool shouldRun;
        std::thread t1;
        std::string error;
        std::string username;
        bool isInit;
        Display* display;
};