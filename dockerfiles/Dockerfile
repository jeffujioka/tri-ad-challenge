FROM ubuntu:20.04

RUN apt-get update && apt-get install --yes software-properties-common \
      && add-apt-repository --yes ppa:ubuntu-toolchain-r/test \
      && apt-get install --yes build-essential gcc-10 g++-10 gdb make wget \
      && cd /tmp \
      && wget https://github.com/Kitware/CMake/releases/download/v3.18.2/cmake-3.18.2-Linux-x86_64.sh \
      && mkdir -p /opt/cmake/cmake-3.18.2 \
      && chmod +x cmake-3.18.2-Linux-x86_64.sh \
      && ./cmake-3.18.2-Linux-x86_64.sh --skip-license --prefix=/opt/cmake/cmake-3.18.2 

ENV PATH="/opt/cmake/cmake-3.18.2/bin:${PATH}"