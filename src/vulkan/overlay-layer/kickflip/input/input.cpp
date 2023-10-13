#include "input.hpp"


InputManager::InputManager()
{
    isInit = false;
    shouldRun = true;
    in.keys[Keys::SHIFT] = 0; //this sucks ass
                 
      in.keys[Keys::TAB] = 0;
                   
      in.keys[Keys::MOUSE1] = 0; 
      in.keys[Keys::MOUSE2] = 0;    
      in.keys[Keys::TILDE] = 0;        
      in.keys[Keys::LALT] = 0;
    t1 = std::thread(&InputManager::GetInput, this);
}

InputManager::~InputManager()
{
    shouldRun = false;
    t1.join(); //detach()?
    
}

bool InputManager::hasInitialized()
{
    return isInit;
}

bool InputManager::supports_key_events(const int &fd)
{
   unsigned long evbit = 0;
   ioctl(fd, EVIOCGBIT(0, sizeof(evbit)), &evbit);
   return (evbit & (1 << EV_KEY));
}
void InputManager::GetInput()
{
   char eventPathStart[] = "/dev/input/event";
   
   for (auto &i : std::filesystem::directory_iterator("/dev/input"))
   {
      if (i.is_character_file())
      {
         std::string view(i.path());
         if (view.compare(0, sizeof(eventPathStart) - 1, eventPathStart) == 0)
         {
            int evfile = open(view.c_str(), O_RDONLY | O_NONBLOCK);
            if (supports_key_events(evfile))
            {
               // std::cout << "Adding " << i.path() << std::endl;
               poll_fds.push_back({evfile, POLLIN, 0});
            }
            else
            {
               close(evfile);
            }
         }
      }
   }
   poll_fds.resize(9);
   in.error = "got " + std::to_string((int)poll_fds.size()) + "\n";
   input_event ev;
   int number_of_screens;
   int i;
   Bool result;
   Window focused;
   int revert_to;
   Window window_returned;
   int root_x, root_y;
   int win_x, win_y;
   unsigned int mask_return;

   int fd, bytes;
   unsigned char data[3];
   const char *pDevice = "/dev/input/mice";
  
   // Open Mouse
   fd = open(pDevice, O_RDWR);
   if (fd == -1)
   {
      in.error += "Couldn't open " + std::string(pDevice);
   }
   int left, middle, right, m4, m5;

   display = XOpenDisplay(NULL);
   assert(display);
   XSetErrorHandler(_XlibErrorHandler);

   XGetInputFocus(display, &focused, &revert_to);
   assert(focused);
  // XSelectInput(display, focused, KeyPressMask | KeyReleaseMask);

   // https://stackoverflow.com/questions/16185286/how-to-detect-mouse-click-events-in-all-applications-in-x11
   // https://stackoverflow.com/questions/59969881/how-to-know-keypresses-using-x11-in-c
    in.keys[Keys::MOUSE2] = 0;  
     in.keys[Keys::MOUSE1] = 0;  
     in.keys[Keys::MOUSE4] = 0; 
     in.keys[Keys::MOUSE5] = 0; 
   isInit = true;
   int refreshed = 0;
   while (shouldRun)
   {
     
                  
      
      poll(poll_fds.data(), poll_fds.size(), -1);
      for (auto &pfd : poll_fds)
      {
         if (pfd.revents & POLLIN)
         {
            if (read(pfd.fd, &ev, sizeof(ev)) == sizeof(ev))
            {
               if (ev.type == EV_KEY && ev.value == 1 && ev.code < 0x100)
               {
                  switch (ev.code)
                  { //https://github.com/torvalds/linux/blob/master/include/uapi/linux/input-event-codes.h
                  case KEY_LEFTSHIFT:
                     in.keys[Keys::SHIFT] = 1;
                     break;
                  case KEY_TAB:
                     in.keys[Keys::TAB] = 1;
                     break;
                  case KEY_L:
                     in.keys[Keys::LALT] = 1;
                     break;
                  case KEY_BACKSLASH:
                     in.keys[Keys::TILDE] = 1;
                     break;
                  default:
                      in.keys[Keys::SHIFT] = 0; //this sucks ass
                 
                     in.keys[Keys::TAB] = 0;
                   
                 
                     in.keys[Keys::LALT] = 0;
                
                  }
               }
            }
         }
      }
      
      bytes = read(fd, data, sizeof(data));
      if (bytes > 0)
      {
         left = data[0] & 0x1;
         right = data[0] & 0x2;
         middle = data[0] & 0x4;
         m4 = data[0] & 0xa; //000a
         m5 = data[0] & 0xc; //0c00 or //0008
      }
      in.keys[Keys::MOUSE1] = (left > 0 ? 1 : 0);
      in.keys[Keys::MOUSE2] = (right > 0 ? 1 : 0);
      in.keys[Keys::MOUSE4] = (m4 > 8 ? 1 : 0);
      in.keys[Keys::MOUSE5] = (m5> 8 ? 1 : 0);
      if(in.keys[Keys::MOUSE4] && in.keys[Keys::MOUSE2])
         in.keys[Keys::MOUSE2] = 0;
      if(refreshed = 1000){
         refreshed = 0;
          XGetInputFocus(display, &focused, &revert_to);
      }
     
      result = XQueryPointer(display, focused, &window_returned,
                             &window_returned, &root_x, &root_y, &win_x, &win_y,
                             &mask_return);
      in.mouse.x = win_x;
      in.mouse.y = win_y;


      refreshed++;
   } //shoudlRUn

   XCloseDisplay(display);
   for (auto &pfd : poll_fds)
   {
      close(pfd.fd);
   }
}

int InputManager::GetNumPolls(){
    return poll_fds.size();
}

bool InputManager::Mouse(int n){
    switch(n){
        case 1:
            return in.keys[Keys::MOUSE1]; 
        case 2:  
            return in.keys[Keys::MOUSE2]; 
        case 4:
            return in.keys[Keys::MOUSE4];
         case 5:
            return in.keys[Keys::MOUSE5];
        default:
            break;
    }
    return false;
}

bool InputManager::MenuKey(){
    return in.keys[Keys::TILDE];
}
bool InputManager::ShouldOpenMenu(){
    if(in.keys[Keys::TILDE] > 0){
        in.keys[Keys::TILDE] = 0;
        return true;
    }
    return false;
}

ImVec2 InputManager::MousePos(){
    return ImVec2(in.mouse.x,in.mouse.y);
}
void InputManager::Shutdown(){
    shouldRun = false;
}