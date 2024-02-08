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
  uuid-dev \
  lcov \
  valgrind \
  xorg \
  qt6-base-dev qt6-webengine-dev qt6-webengine* libqt6webenginecore6* libegl1-mesa-dev libx11-xcb-dev libxkbcommon-dev \
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
  python3-pip \
  libboost-system-dev \
  libboost-python-dev \
  libboost-filesystem-dev \
  zlib1g-dev \
  automake \
  autoconf \
  libgtk-3-dev \
  ninja-build \
  libhwloc-dev \
  libssl-dev \
  libusb-1.0-0-dev \
  pkg-config \
  libboost-program-options-dev \
  libboost-date-time-dev \
  libboost-test-dev  \
  lsb-core

# for cmake warning, Could NOT find WrapVulkanHeaders, install the Vulkan separately by following the instruction from https://vulkan-tutorial.com/Development_environment#page_Vulkan-Packages  
wget -qO- https://packages.lunarg.com/lunarg-signing-key-pub.asc | sudo tee /etc/apt/trusted.gpg.d/lunarg.asc
sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-jammy.list http://packages.lunarg.com/vulkan/lunarg-vulkan-jammy.list
sudo apt update
sudo apt install vulkan-sdk -y

sudo apt install -y libunwind-dev
sudo apt install -y --no-install-recommends libgoogle-perftools-dev

sudo ln -sf /usr/bin/g++-11 /usr/bin/g++
sudo ln -sf /usr/bin/gcc-11 /usr/bin/gcc
sudo ln -sf /usr/bin/gcov-11 /usr/bin/gcov
curl -LO http://archive.ubuntu.com/ubuntu/pool/main/libf/libffi/libffi6_3.2.1-8_amd64.deb
sudo dpkg -i libffi6_3.2.1-8_amd64.deb

