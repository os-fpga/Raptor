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

  git clone git@github.com:RapidSilicon/Raptor.git
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
If you would like to record (commit) an update of the submodule then the procedure is as following:
```
  cd Raptor
  git checkout main
  git pull
  git submodule update --recursive                  // Note that this command will update all submodules recursively and in case if some submodules are not checked out (cloned) then it will clone them as well.
  git checkout -b NEW-BRANCH-NAME                   // NEW-BRANCH-NAME is the name of your branch that should be used for merging into main
  cd SUBMODULE                                      // SUBMODULE is the name of the submodule: ex - yosys_verific_rs
  git checkout main
  git pull
  cd -
  git add SUBMODULE
  git commit -m "Updated SUBMODULE."
  git push --set-upstream origin NEW-BRANCH-NAME
```
After the last command you should be able to create Pull Request in Raptor repository.

Note 1: **Always add committing files/submodules explicitly. Do not use** `git commit . -m "MESSAGE"` **or** `git add .` **commands.**

Note 2: Use `git status` command to check your working tree status: what is added for commit, what is modified, what is untracked.

    
 2) NIGHTLY BUILD ACCESS:
 The Rapid Silicon build server automatically produces nightly builds of Raptor that can be used quickly for testing.  The instructions below detail how to connect to the automated build server and run Raptor in that environment.
```
Log in to Fremont/Arbutus server:
  Run VPN with NetExtender
  Run NX and connect server: nx01.rapid.local

SSH with X forwarding to compute server:
  ssh -X sim01, or ssh -X sim02, or ssh -X sw01 or ssh -X sw03

Load nightly built raptor:
module load fpga_tools/raptor/latest

Launch raptor:
raptor
```

 3) BUILD RAPTOR ON THE FREMONT SERVER:
 The instructions detail how to build Raptor from source code but on an IT-managed Linux host.  First, follow the instructions below to configure the build environment.  Then follow instructions described in "BUILD YOURSELF Raptor LOCALLY ON YOUR MACHINE" to clone and build Raptor.
```
SSH with X forwarding to compute server:
  ssh -X sim01, or ssh -X sim02, or ssh -X sw01 or ssh -X sw03

Clone and build the raptor on compute server:
  Refer to section "1)"

Load build_env raptor:
module load fpga_tools/raptor/build_env
```
