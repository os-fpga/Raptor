# Install required dependencies for CentOS systems
yum update -y
yum group install -y "Development Tools" 
yum install -y epel-release 
curl -C - -O https://cmake.org/files/v3.15/cmake-3.15.7-Linux-x86_64.tar.gz
tar xzf cmake-3.15.7-Linux-x86_64.tar.gz
ln -s $PWD/cmake-3.15.7-Linux-x86_64/bin/cmake /usr/bin/cmake
yum install -y openssh-server openssh-clients
yum install -y centos-release-scl
yum install -y devtoolset-9
yum install -y devtoolset-9-toolchain
yum install -y devtoolset-9-gcc-c++
scl enable devtoolset-9 bash
yum install -y tcl
yum install -y tcl-dev
yum install -y make
yum install -y flex
yum install -y bison
yum install -y readline-devel
yum install -y swig
yum install -y which
yum install -y google-perftools
yum install -y gperftools gperftools-devel
yum install -y uuid-devel
yum install -y valgrind
yum install -y python3
yum install -y xorg-x11-server-Xorg xorg-x11-xauth xorg-x11-apps 
yum install -y xorg-x11-server-Xvfb
yum install -y mesa-libGL-devel
yum install -y libxcb libxcb-devel xcb-util xcb-util-devel libxkbcommon-devel libxkbcommon-x11-devel
yum install -y xcb-util-image-devel xcb-util-keysyms-devel xcb-util-renderutil-devel xcb-util-wm-devel
yum install -y autoconf
wget https://download-ib01.fedoraproject.org/pub/epel/7/x86_64/Packages/e/epel-release-7-14.noarch.rpm
rpm -Uvh epel-release*rpm
yum install -y tcllib
yum install -y bison
yum install -y flex
yum install -y readline-devel
yum install -y gawk
yum install -y tcl-devel
yum install -y libffi-devel
yum install -y git
yum install -y graphviz
yum install -y pkgconfig
yum install -y python3
yum install -y boost-system
yum install -y boost-python
yum install -y boost-filesystem
yum install -y zlib-devel
yum install http://repo.okay.com.mx/centos/7/x86_64/release/okay-release-1-1.noarch.rpm
yum install -y ninja-build
yum install -y wget


ln -s $PWD/cmake-3.15.7-Linux-x86_64/bin/ctest /usr/bin/ctest
echo 'QMAKE_CC=/opt/rh/devtoolset-9/root/usr/bin/gcc' >> $GITHUB_ENV
echo 'QMAKE_CXX=/opt/rh/devtoolset-9/root/usr/bin/g++' >> $GITHUB_ENV
echo 'PATH=/usr/local/Qt-5.15.0/bin:/usr/lib/ccache:'"$PATH" >> $GITHUB_ENV

if [ -f buildqt5-centos7-gcc.tgz ]
then
  echo "Found QT build artifact, untarring..."
  tar xvzf buildqt5-centos7-gcc.tgz
fi

echo "Downloading QT..."
curl -L http://download.qt.io/official_releases/qt/5.15/5.15.0/single/qt-everywhere-src-5.15.0.tar.xz --output qt-everywhere-src-5.15.0.tar.xz
tar -xf qt-everywhere-src-5.15.0.tar.xz

if [ -d "buildqt5" ] 
then
  echo "Installing QT..."
  cd buildqt5  
  make install
  cd ..
else
  echo "Building QT..."
  mkdir buildqt5
  cd buildqt5
  source /opt/rh/devtoolset-9/enable
  ../qt-everywhere-src-5.15.0/configure -opensource -confirm-license -xcb -xcb-xlib -bundled-xcb-xinput -no-compile-examples -nomake examples
  make -j 2
  echo "Installing QT..."
  make install
  cd ..
fi
