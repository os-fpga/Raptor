# Install required dependencies for CentOS systems
yum update -y
yum install -y "Development Tools"
yum install -y python3
yum install -y ninja-build
yum install -y glibc-devel.i68
yum install -y libevent-devel
yum install -y json-c-devel
yum install -y flex
yum install -y bison
yum install -y verilator

# Install required Python3 packages.
pip3 install \
  setuptools \
  requests \
  pexpect \
  meson

# Download/Install LiteX.
wget https://raw.githubusercontent.com/enjoy-digital/litex/master/litex_setup.py
chmod +x litex_setup.py
./litex_setup.py --init
./litex_setup.py --install

# Download/Install RISC-V GCC toolchain.
./litex_setup.py --gcc=riscv
mkdir /usr/local/riscv
cp -r riscv64-*/* /usr/local/riscv
