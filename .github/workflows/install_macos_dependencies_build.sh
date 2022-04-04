# Install required dependencies for Mac OS systems
brew install qt5 \
    bison \
    flex \
    gawk \
    libffi \
    pkg-config \
    bash \
    readline \
    ninja
export PATH="/usr/local/opt/qt@5/bin:/usr/local/opt/bison/bin:$PATH"
export LDFLAGS="-L/usr/local/opt/qt@5/lib"
export CPPFLAGS="-I/usr/local/opt/qt@5/include"
