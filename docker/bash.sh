xhost +
docker run --rm -ti --net=host -e DISPLAY=:0 -v $PWD/..:/cacd_host -it tudelft-space-cacd /bin/bash

