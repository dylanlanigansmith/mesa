#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>


#include "mouse.hpp"
#define errexit(str, args...) do { \
        perror(str); \
        exit(EXIT_FAILURE); \
    } while(0)
 
mousetrap::mousetrap(){
    ready = false;
    err = "init.. ";
    fd = open(UINPUT, O_WRONLY | O_NONBLOCK);
    if(fd < 0 )
        err.append(xs(" \n failed to open /dev/uinput"));
    if(ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0)
         err.append(xs("\n error: ioctl(fd, UI_SET_EVBIT, EV_KEY)"));
    if(ioctl(fd, UI_SET_KEYBIT, BTN_LEFT) < 0)
         err.append(xs("\n  error: ioctl(fd, UI_SET_KEYBIT, BTN_LEFT)"));
 
    if(ioctl(fd, UI_SET_EVBIT, EV_REL) < 0)
         err.append(xs("\n  error: ioctl(fd, UI_SET_EVBIT, EV_REL)"));
    if(ioctl(fd, UI_SET_RELBIT, REL_X) < 0)
         err.append(xs("\n  error: ioctl(fd, UI_SET_RELBIT, REL_X)"));
    if(ioctl(fd, UI_SET_RELBIT, REL_Y) < 0)
         err.append(xs("\n  error: ioctl(fd, UI_SET_RELBIT, REL_Y)"));
     memset(&uidev, 0, sizeof(uidev));
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE,"%s", xs("uinput-game").c_str());
    uidev.id.bustype = BUS_USB;
    uidev.id.vendor  = 0x1;
    uidev.id.product = 0x1;
    uidev.id.version = 1;

    if(write(fd, &uidev, sizeof(uidev)) < 0)
         err.append(xs(" \n error: write &uidev"));
 
    if(ioctl(fd, UI_DEV_CREATE) < 0)
         err.append(xs(" \n error: ioctl UI_DEV_CREATE"));
    if(err.length() > 10)
        return;
     err = xs("init success");
    ready = true;
    return;
}

bool mousetrap::setmouse(int x, int y){
        struct input_event ev;
        err = "setting: " + std::to_string(x) + " " + std::to_string(y);
        bool fail = false;
        if(!ready){
            err.append(" not initialized (??)"); return fail;
        }
        memset(&ev, 0, sizeof(struct input_event));
        ev.type = EV_REL;
        ev.code = REL_X;
        ev.value = x;
        if(write(fd, &ev, sizeof(struct input_event)) < 0){
             err.append(xs(" error: write rel_x "));
             fail = true;
        }
 
        memset(&ev, 0, sizeof(struct input_event));
        ev.type = EV_REL;
        ev.code = REL_Y;
        ev.value = y;
        if(write(fd, &ev, sizeof(struct input_event)) < 0){
             err.append(xs(" error: write rel_y "));
             fail = true;
        }
 
        memset(&ev, 0, sizeof(struct input_event));
        ev.type = EV_SYN;
        if(write(fd, &ev, sizeof(struct input_event)) < 0){
             err.append(xs(" error: write syn "));
             fail = true;
        }
        return fail;
}

mousetrap::~mousetrap(){
    if(ioctl(fd, UI_DEV_DESTROY) < 0)
        err.append(xs("error: ioctl UI_DEV_DESTROY"));
    close(fd);
}

