#!/bin/bash

docker run  --mount type=bind,source="$(pwd)/",target=/project \
            -w /project -it --rm \
            gcc-10.2_with_cmake-3.18.2 \
            bash -c "$1"
