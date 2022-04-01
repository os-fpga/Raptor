# Install required dependencies for Ubuntu systems
sudo apt-get update -qq
sudo apt install -y \
  g++-9 \
  tclsh \
  cmake \
  build-essential \
  swig \
  google-perftools \
  libgoogle-perftools-dev \
  uuid-dev \
  lcov \
  valgrind \
  xorg \
  qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools qtdeclarative5-dev \
  xvfb \
  tcllib \
  bison \
  flex \
  libreadline-dev \
  gawk \
  tcl-dev \
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
  ninja-build 
  
sudo ln -sf /usr/bin/g++-9 /usr/bin/g++
sudo ln -sf /usr/bin/gcc-9 /usr/bin/gcc
sudo ln -sf /usr/bin/gcov-9 /usr/bin/gcov
