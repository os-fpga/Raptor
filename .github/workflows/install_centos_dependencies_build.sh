set -e
# Install required dependencies for CentOS systems
yum install -y https://repo.ius.io/ius-release-el7.rpm
yum update -y
yum group install -y "Development Tools" 
yum install -y epel-release 
curl -C - -O https://cmake.org/files/v3.20/cmake-3.20.0-linux-x86_64.tar.gz
tar xzf cmake-3.20.0-linux-x86_64.tar.gz
ln -s $PWD/cmake-3.20.0-linux-x86_64/bin/cmake /usr/bin/cmake
yum install -y openssh-server openssh-clients
yum -y remove git git-*
yum -y install https://packages.endpointdev.com/rhel/7/os/x86_64/endpoint-repo.x86_64.rpm
yum install -y git
yum install -y centos-release-scl-rh
yum install -y devtoolset-11
yum install -y devtoolset-11-toolchain
yum install -y devtoolset-11-gcc-c++
scl enable devtoolset-11 bash
yum install -y tcl
yum install -y make
yum install -y flex
yum install -y bison
yum install -y readline-devel
yum remove -y swig
yum install -y swig3
yum install -y which
yum install -y java-11-openjdk-devel
yum install -y google-perftools
yum install -y gperftools gperftools-devel
yum install -y uuid-devel
yum install -y valgrind redhat-lsb-core
yum install -y python36u python36u-libs python36u-devel python36u-pip

pip3 install orderedmultidict
pip3 install psutil

yum install -y xorg-x11-server-Xorg xorg-x11-xauth xorg-x11-apps 
yum install -y xorg-x11-server-Xvfb
yum install -y mesa-libGL-devel
yum install -y libxcb libxcb-devel xcb-util xcb-util-devel libxkbcommon-devel libxkbcommon-x11-devel
yum install -y xcb-util-image-devel xcb-util-keysyms-devel xcb-util-renderutil-devel xcb-util-wm-devel
yum install -y autoconf wget
#wget https://download-ib01.fedoraproject.org/pub/epel/7/x86_64/Packages/e/epel-release-7-14.noarch.rpm
#rpm -Uvh epel-release*rpm
yum install -y tcllib
yum install -y gawk
yum install -y tcl-devel
yum install -y libffi-devel
yum install -y git
yum install -y graphviz
yum install -y pkgconfig
yum install -y boost-system
yum install -y boost-python
yum install -y boost-filesystem
yum install -y zlib-devel
#yum install http://repo.okay.com.mx/centos/7/x86_64/release/okay-release-1-1.noarch.rpm
yum install -y ninja-build
yum install -y zip unzip
yum install -y gtk3-devel
yum install -y openssl-devel hwloc-devel
yum install -y libusbx-devel libusb-devel
yum install -y pkgconfig

ln -s $PWD/cmake-3.16.9-Linux-x86_64/bin/ctest /usr/bin/ctest

echo "Downloading QT..."
curl -L https://github.com/RapidSilicon/post_build_artifacts/releases/download/v0.1/Qt-6.5.1.tar.gz --output qt-everywhere-src-6.5.1.tar.gz
tar -xzf qt-everywhere-src-6.5.1.tar.gz
mv Qt-6.5.1 /usr/local/
yum clean all
rm -rf qt-everywhere-src-6.5.1.tar.gz

