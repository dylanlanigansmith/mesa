#pragma once
#include <codecvt>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <X11/Xlib.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <variant>
#include <poll.h>
#include <signal.h>
#include <fstream>
#include <filesystem>
#include <string_view>
#include <unistd.h>
#include <stdio.h>
#include <malloc.h>
#include <memory>
#include <stdexcept>
#include <array>
#include <cstdio>
#include <vector>
#include <sstream>
#include <iomanip>
#include "imgui.h"
#include <thread>
#include <string>
#include <signal.h>
#include <cinttypes>
#include <optional>
#include <exception>
#include <type_traits>
#include <cstdio>
#include <cmath>
#include <iostream>
#include <typeinfo>
#include <iterator> 
#include <chrono>
#include <unordered_map>
#include <functional>

#include "xorstr.hpp"


#define FOV 90
#define xc(c) xs(c).c_str()

#define KFVERSION "2.2.0s"

#define COMBINE(x, y) x##y
#define COMBINE2(x, y) COMBINE(x, y)

#define PAD_CLASS(sz)                           \
 private:                                       \
  std::uint8_t COMBINE2(pad_, __COUNTER__)[sz]; \
                                                \
 public:
