# Copyright © 2019 Intel Corporation

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

overlay_shaders = [
  'overlay.frag',
  'overlay.vert',
]
overlay_spv = []
foreach s : ['overlay.frag', 'overlay.vert']
  overlay_spv += custom_target(
    s + '.spv.h', input : s, output : s + '.spv.h',
    command : [prog_glslang, '-V', '-x', '-o', '@OUTPUT@', '@INPUT@'] + glslang_quiet)
endforeach

vklayer_files = files(
  'overlay.cpp',
  'kickflip/sdk/sdk.cpp',
  'kickflip/sdk/getvar.cpp',
  'overlay_params.c',
  'kickflip/mem/memory.cpp',
  'kickflip/input/input.cpp',
  'kickflip/frame.cpp',
  'kickflip/mem/pointer.cpp',
  'kickflip/mem/virtual.cpp',
  'kickflip/schema/schemasystem.cpp',
  'kickflip/schema/schemamgr.cpp',
  'kickflip/newframe.cpp',
  'kickflip/utils.cpp',
  'kickflip/sdk/enginetrace.cpp',
  'kickflip/sdk/sdk.cpp',
  'kickflip/ESP/esp.cpp',
  'kickflip/sdk/types/CHandle.cpp',
  'kickflip/cfg/config.cpp',
  'kickflip/GUI/gui.cpp',
  'kickflip/sdk/interface/IEngineClient.cpp',
  'kickflip/mouse/mouse.cpp',
  'kickflip/AIM/aim.cpp',
  'kickflip/sdk/classes/classes.cpp'
  
)
#no_override_init_args
vklayer_mesa_overlay = shared_library(
  'VkLayer_MESA_overlay',
  vklayer_files, overlay_spv, sha1_h,
  c_args : [],
  gnu_symbol_visibility : 'hidden',
  dependencies : [idep_vulkan_util, idep_mesautil, vulkan_wsi_deps, libimgui_core_dep, dep_dl],
  include_directories : [inc_include, inc_src],
  link_args : cc.get_supported_link_arguments(['-Wl,-Bsymbolic-functions', '-Wl,-z,relro -lX11']),
  install : true
)

install_data(
  files('VkLayer_FLIP_overlay.json'),
  install_dir : join_paths(get_option('datadir'), 'vulkan', 'explicit_layer.d'),
)

install_data(
  'mesa-overlay-control.py',
  install_dir : get_option('bindir'),
  install_mode : 'r-xr-xr-x',
)
