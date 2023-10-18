#include "input.hpp"

#include "../frame.hpp"
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
    t1 = std::thread(&InputManager::NewGetInput, this);
    fallback_kb = 7; fallback_mb = 4;
    found_mb = found_kb = -1;
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
 std::string InputManager::getEventName(int fd){

   char name[256]= "Unknown";
   if(ioctl(fd, EVIOCGNAME(sizeof(name)), name) < 0) {
    kf->Log("evdev ioctl err");
    
   }
   return std::string(name);
 }

 bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

void InputManager::getEventNumbers(){
   const char* eventPathStart = "/dev/input/event";
  

   const std::string dylanmouse = xs("Logitech G500s Laser Gaming Mouse");
   const std::string dylankeyboard = xs("Kingston HyperX Alloy FPS Pro Mechanical Gaming Keyboard"); 
   const std::string haydenmouse = xs("Logitech USB Receiver");
   const std::string haydenkeyboard = xs("KBDFans DZ60 Keyboard"); 
   const std::string nickmouse = xs("Logitech G604");
   const std::string nickkeyboard = xs("SONiX USB Keyboard"); 

   char* user = getenv("USER");
   if(strlen(user) == 0)
      return;
   username = user;
   std::unordered_map<std::string, int> events;
   int low_hype = 25;
   for (auto &i : std::filesystem::directory_iterator("/dev/input"))
   {
      if (i.is_character_file())
      {
         std::string view(i.path());
       
         if (view.find("event") != std::string::npos) //in dir fr
         {
            int num = -1;
            std::string last2 =  view.substr(view.length() - 2 ) ;
            if(is_number(last2)){
               num = stoi(last2);
            }
            else{
               char number = view.back(); 
               num = number - '0';
            }
            

           
            int evfile = open(view.c_str(), O_RDONLY );
            std::string eventname = getEventName(evfile);
           // kf->Log("- " + eventname + " #" + std::to_string(num));
            if(eventname.find("G604") != std::string::npos){
               found_mb = num;
            }
            if(eventname.find("Alloy FPS") != std::string::npos){
              if(num < low_hype)
               low_hype = num;
            }
            if(auto search = events.find(eventname); search != events.end()){
               if(num > events.at(eventname) )
                  continue; 
            }
            else
               events.insert({eventname, num});
            close(evfile);
         }
      }
   }
   

   if(strcmp(user, "dylan") == 0){
      if(auto search = events.find(dylanmouse); search != events.end()){
         found_mb = events.at(dylanmouse);
         kf->Log("Found Mouse" );
      }
      if(auto search = events.find(dylankeyboard); search != events.end()){
         found_kb = events.at(dylankeyboard);
         if(low_hype < found_kb){
            found_kb = low_hype;
            kf->Log("Low Hype" );
         }
            
          kf->Log("Found Keyboard" );
      }
      if(found_kb < 0 || found_mb < 0 ){
         kf->Log(xs("Could not auto-obtain input devices.")); 
      }
   }
   else if(strcmp(user, "nicholas") == 0){
      if(auto search = events.find(nickmouse); search != events.end()){
         found_mb = events.at(nickmouse);
          kf->Log("Found Mouse" );
      }
      if(auto search = events.find(nickkeyboard); search != events.end()){
         found_kb = events.at(nickkeyboard);
         kf->Log("Found Keyboard" );
      }
      if(found_kb < 0 || found_mb < 0 ){
         kf->Log(xs("Could not auto-obtain input devices.")); 
      }
   }
   else if(strcmp(user, "hayden") == 0){
      if(auto search = events.find(haydenmouse); search != events.end()){
         found_mb = events.at(haydenmouse);
          kf->Log("Found Mouse" );
      }
      if(auto search = events.find(haydenkeyboard); search != events.end()){
         found_kb = events.at(haydenkeyboard);
         kf->Log("Found Keyboard" );
      }
      if(found_kb < 0 || found_mb < 0 ){
         kf->Log(xs("Could not auto-obtain input devices."));
        
      }
   }

     if(found_kb < 0 || found_mb < 0 ){
      kf->Log(xs("No Luck Finding For ") + username);
      return;
     }



   kf->Log("Found kb: " + std::to_string(found_kb));
   kf->Log("Found mb: " + std::to_string(found_mb));

}

void InputManager::GetInput()
{
   
   const char* eventPathStart = "/dev/input/event";
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
   
   int i;
   
   int result;
   Window focused;
   int revert_to;
   Window window_returned;
   int root_x, root_y;
   int win_x, win_y;
   unsigned int mask_return;
   int number_of_screens;

   display = XOpenDisplay(NULL);
   assert(display);
   XSetErrorHandler(_XlibErrorHandler);

   XGetInputFocus(display, &focused, &revert_to);
   assert(focused);

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
      
      bytes = read(fd, data, sizeof(data)); // BTN_4 BTN_LEFT 
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

         //EVENT 4 - mouse and EVENT 7 - key
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
//#define USE_ENV

void InputManager::NewGetInput(){


   
   long ev_k = fallback_kb;
   long ev_m = fallback_mb;
   const std::string eventPathStart = "/dev/input/event";


#ifdef USE_ENV
   char* num_k = getenv("KF_KBEV");
   
   char* fail;
   ev_k = strtol(num_k, &fail, 10);
   if (*fail) {
      in.error += "EnvVar [k] - fallback ";
      ev_k = fallback_kb;
   }

   char* num_m = getenv("KF_MBEV"); //may need to add to cs2.sh :/
   kf->Log(num_m); 
   kf->Log(num_k); 
   char* fail2;
   ev_m = strtol(num_m, &fail2, 10); //FIX THIS (JUST USE STD::STRING);
   if (*fail2) {
      in.error += "EnvVar [k] - fallback ";
      ev_m = fallback_mb;
   }
   // export KF_KBEV=7
  // export KF_MBEV=4
#endif
#ifndef USE_ENV
    getEventNumbers();
    if(found_kb < 0 || found_mb < 0 ){
      kf->Log(xs("using fallback values for kb/mb"));
    }
    else{
      ev_k = found_kb;
      ev_m = found_mb; 
    }
#endif
   std::string k_path = eventPathStart + std::to_string(ev_k);
   std::string m_path = eventPathStart + std::to_string(ev_m);
  // k_path = "/dev/input/event7"; m_path = "/dev/input/event4"; 
   if(!std::filesystem::exists(k_path) || !std::filesystem::exists(m_path)){
      in.error += " No Event path: " + k_path;
      in.error += " No Event path: " + m_path;
      kf->Log(error);
      return;
   }
   getEventNumbers();
   int fd_k = open(k_path.c_str(), O_RDONLY ); // | O_NONBLOCK
   int fd_m = open(m_path.c_str(), O_RDONLY ); // | O_NONBLOCK

   if(fd_k < 0 || fd_m < 0){
      in.error += " Couldn't Open Event";
      kf->Log(error);
      return;
   }
   input_event event_k;
   input_event event_m;
   
   //Init KBM Done

   //X Init
   int result;
   Window focused;
   int revert_to;
   Window window_returned;
   int root_x, root_y;
   int win_x, win_y;
   unsigned int mask_return;
   int number_of_screens;

   display = XOpenDisplay(NULL);
   assert(display);
   XSetErrorHandler(_XlibErrorHandler);

   XGetInputFocus(display, &focused, &revert_to);
   assert(focused);



   //DONE INIT
   isInit = true;
   kf->Log("IM: Init");
   int refreshedFocus = 0;
   

   while (shouldRun)
   {
      pollfd fds[2] = { {fd_k, POLLIN, 0}, {fd_m, POLLIN, 0}};
      poll(fds, 2, -1); //timeout 
      if(fds[0].revents & POLLIN) //KB
      {
         if(read(fds[0].fd, &event_k, sizeof(event_k)) == sizeof(event_k))
         {
            if(event_k.type == EV_KEY && event_m.code < 0x100)
            {
               switch (event_k.code)
               {
                  case KEY_BACKSLASH:
                     in.keys[Keys::TILDE] = event_k.value; 
                     break;
                
               }
            }
         }
      }
      if(fds[1].revents & POLLIN) //MB
      {
         if(read(fds[1].fd, &event_m, sizeof(event_m)) == sizeof(event_m))
         {
            if(event_m.type == EV_KEY && event_m.code > 0x100) //https://kernel.org/doc/html/v4.12/input/event-codes.html
            {
               
               switch (event_m.code)
               {
                  case BTN_LEFT:
                     in.keys[Keys::MOUSE1] = event_m.value;  break;
                  case BTN_RIGHT:
                     in.keys[Keys::MOUSE2] = event_m.value; break;
                  case BTN_EXTRA:
                     in.keys[Keys::MOUSE5] = event_m.value; break;
                  case BTN_SIDE: 
                     in.keys[Keys::MOUSE4] = event_m.value; break;
                  
                 
                  default:
                     break;
               }
            }
         }
      } //end poll k

      if(refreshedFocus == 1000){
         refreshedFocus = 0;
          
      }
      XGetInputFocus(display, &focused, &revert_to);
      result = XQueryPointer(display, focused, &window_returned,
                             &window_returned, &root_x, &root_y, &win_x, &win_y,
                             &mask_return);
      in.mouse.x = win_x;
      in.mouse.y = win_y;
      refreshedFocus++;

   } // end loop

   XCloseDisplay(display);
   close(fd_k); close(fd_m);
   //shutdown
}