#!/bin/bash

#VK_INSTANCE_LAYERS=VK_LAYER_MESA_overlay VK_INSTANCE_LAYERS=VK_LAYER_FLIP_overlay


unset VK_INSTANCE_LAYERS
cd ~/code/kickflip/mesa
echo "Building.."
sudo ninja -C builddir/
chmod 755 ./src/vulkan/overlay-layer/*.json
cp ./src/vulkan/overlay-layer/*.json ~/share/vulkan/implicit_layer.d 
cp ./builddir/src/vulkan/overlay-layer/*.so ~/libflipover.so
echo "Done-zo"

export VK_INSTANCE_LAYERS=VK_LAYER_FLIP_overlay && export VK_LAYER_MESA_OVERLAY_CONFIG=position=top-right,control=kickflip
