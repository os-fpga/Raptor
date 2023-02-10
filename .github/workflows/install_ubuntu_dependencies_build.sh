set -e
# Install required dependencies for Ubuntu systems
sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt-get update -qq
sudo apt install -y \
  g++-11 gcc-11 \
  tclsh \
  tcl-dev \
  cmake \
  build-essential \
  swig \
  google-perftools \
  libgoogle-perftools-dev \
  uuid-dev \
  lcov \
  valgrind \
  xorg \
  qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools \
  xvfb \
  tcllib \
  bison \
  flex \
  libreadline-dev \
  gawk \
  libffi-dev \
  git \
  graphviz \
  xdot \
  pkg-config \
  python3 \
  libboost-system-dev \
  libboost-python-dev \
  libboost-filesystem-dev \
  zlib1g-dev \
  automake \
  autoconf \
  libgtk-3-dev \
  ninja-build \
  libssl-dev
# qtdeclarative5-dev

sudo apt -y install lsb-core

sudo ln -sf /usr/bin/g++-11 /usr/bin/g++
sudo ln -sf /usr/bin/gcc-11 /usr/bin/gcc
sudo ln -sf /usr/bin/gcov-11 /usr/bin/gcov
curl -LO http://archive.ubuntu.com/ubuntu/pool/main/libf/libffi/libffi6_3.2.1-8_amd64.deb
sudo dpkg -i libffi6_3.2.1-8_amd64.deb

