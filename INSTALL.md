# Raptor
Rapid Silicon complete Software solution

## INSTALL GUIDE

 * Current OS Support: Centos7, Ubuntu 20.04, Ubuntu 21.04
 * Planned OS Support: MacOS, Windows

3 ways to get access to Raptor:

 1) BUILD YOURSELF Raptor LOCALLY ON YOUR MACHINE:

 * [`Ubuntu dependencies`](.github/workflows/install_ubuntu_dependencies_build.sh)
 * [`Centos dependencies`](.github/workflows/install_centos_dependencies_build.sh)
 * [`MacOS dependencies`](.github/workflows/install_macos_dependencies_build.sh)
 * [`Windows Msys2 dependencies`](.github/workflows/main.yml)
 * [`Windows MSVC dependencies`](.github/workflows/main.yml)

```
  Enable github ssh keys:

    Create a SSH key pair with the ssh-keygen command like below
    ssh-keygen -t rsa -b 4096 -C "email@example.com"
    Copy the value of the public SSH key to the clipboard
    Login to GitHub and navigate to your account settings
    Click on the SSH and GPG link
    Click Add Key to register the public SSH key with your account
    Name the key and paste the copied value into the text field
    Save your changes

  git clone https://github.com/RapidSilicon/Raptor.git
  cd Raptor
  git submodule update --init --recursive
  make
  make debug
  make test
  make install
  make test_install
```
    
 2) NIGHTLY BUILD ACCESS:
```
Log in to Fremont/Arbutus server:
Run VPN with NetExtender
Run NX and connect server: nx01.rapid.local
Run pre-build Raptor 
ssh -X sim01, or ssh -X sim02
module load raptor/build_env
module load raptor/latest
raptor ...
```

 3) BUILD RAPTOR ON THE FREMONT SERVER:
```
ssh sw01, sw03
mkdir YOUR_WORK_DIR
cd YOUR_WORK_DIR
module load raptor/build_env
Follow above instructions described in "BUILD YOURSELF Raptor LOCALLY ON YOUR MACHINE" to clone and build your raptor
```
