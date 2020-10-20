#!/bin/bash

cd dockerfiles
docker build --force-rm -t gcc-10.2_with_cmake-3.18.2 .