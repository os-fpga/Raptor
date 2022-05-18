# Raptor
RapidSilicon complete Software solution

OS Support: Centos7, Ubuntu 20.04, Ubuntu 21.04

INSTALL Instructions:

 * [`Ubuntu dependencies`](.github/workflows/install_ubuntu_dependencies_build.sh)
 * [`Centos dependencies`](.github/workflows/install_centos_dependencies_build.sh)
 * [`MacOS dependencies`](.github/workflows/install_macos_dependencies_build.sh)
 * [`Windows Msys2 dependencies`](.github/workflows/main.yml)
 * [`Windows MSVC dependencies`](.github/workflows/main.yml)

```
  git clone https://github.com/RapidSilicon/Raptor.git
  git submodule update --init --recursive
  make
  make debug
  make test
  make install
  make test_install
```
    
