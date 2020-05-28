#!/bin/bash

sh scripts/docker_rm.sh

# if [ "$#" -ne 2 ]; then
#   echo "Usage: $0 IMAGE_NAME CONTAINER_NAME" >&2
#   exit 1
# fi

IMAGE_NAME=krita-deps
CONTAINER_NAME=krita-auto-1

NVIDIA_OPTS=
if [ -e /dev/nvidiactl ]; then
    NVIDIA_OPTS+="--device /dev/nvidia0 --device /dev/nvidiactl --device /dev/nvidia-uvm"
fi

docker run -P -it \
    -v $(pwd)/persistent/:/home/appimage/persistent/:rw \
    -v /tmp/.X11-unix/:/tmp/.X11-unix \
    -v /home/$USER/.Xauthority:/home/appimage/.Xauthority \
    -v /etc/localtime:/etc/localtime:ro \
    -e DISPLAY=$DISPLAY \
    -h $HOSTNAME \
    --cap-add=SYS_PTRACE \
    --security-opt seccomp=unconfined \
    $NVIDIA_OPTS \
    $IMAGE_NAME bash

# --device /dev/dri \
#   --device /dev/snd \

if [ ! -f .container_name ]; then
    echo $2 >.container_name
fi

# cd /tmp/krita-auto-1
# ./bin/run_container krita-deps krita-auto-1
