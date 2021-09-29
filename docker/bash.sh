# docker run -v $PWD/..:/cacd -it tudelft-space-cacd /bin/bash
# docker run --env=DISPLAY --volume=/tmp/.X11-unix:/tmp/.X11-unix --net=host -it tudelft-space-cacd /bin/bash
xhost +
docker run --rm -ti --net=host -e DISPLAY=:0 -it tudelft-space-cacd /bin/bash