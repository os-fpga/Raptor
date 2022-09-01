# Raptor

## How to build Raptor from source code

3 ways to build Raptor from sources:

 1) BUILD YOURSELF Raptor LOCALLY ON YOUR MACHINE:
 This workflow clones the Raptor source code repository and builds it locally on your own test platform.  Please take into acocunt the dependcies in the files listed below.  If you are unsure of package installation status, please run the appropriate script after cloning the repository and prior to building.

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
  make
  make debug
  make test
  make install
  make test_install

To update and build your local repository run the following commands.

  cd Raptor
  git pull
  make UPDATE_SUBMODULES=ON

Note 1: During the build all required submodules would be updated. 
Note 2: The Raptor_Tools submodule would be left uninitialized in all Raptor submodules.
```
    
 2) NIGHTLY BUILD ACCESS:
 The Rapid Silicon build server automatically produces nightly builds of Raptor that can be used quickly for testing.  The instructions below detail how to connect to the automated build server and run Raptor in that environment.
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
 The instructions detail how to build Raptor from source code but on an IT-managed Linux host.  First, follow the instructions below to configure the build environment.  Then follow instructions described in "BUILD YOURSELF Raptor LOCALLY ON YOUR MACHINE" to clone and build Raptor.
```
ssh sw01, sw03
mkdir YOUR_WORK_DIR
cd YOUR_WORK_DIR
module load raptor/build_env
```
