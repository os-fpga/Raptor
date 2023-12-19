# Raptor

## How to Build Raptor from Source Code

There are three ways to build Raptor from sources:

### 1) Build Locally on Your Machine:

Once your local machine is ready, please take into account the dependencies listed below. If you are unsure about the package installation status, go to the Raptor repository on GitHub by clicking [here](https://github.com/RapidSilicon/Raptor/) and install the Raptor build dependencies by running the appropriate script:

- [Ubuntu dependencies](https://github.com/RapidSilicon/Raptor/blob/main/.github/workflows/install_ubuntu_dependencies_build.sh)
- [Centos dependencies](https://github.com/RapidSilicon/Raptor/blob/main/.github/workflows/install_centos_dependencies_build.sh)
- [MacOS dependencies](https://github.com/RapidSilicon/Raptor/blob/main/.github/workflows/install_macos_dependencies_build.sh)
- [Windows Msys2 dependencies](https://github.com/RapidSilicon/Raptor/blob/main/.github/workflows/main.yml)
- [Windows MSVC dependencies](https://github.com/RapidSilicon/Raptor/blob/main/.github/workflows/main.yml)

> [!TIP]
> Copy the script content and paste it into a file created on your local machine, then execute that file as a bash script.

<h4 id="step-to-build-raptor">
Steps to clone and build Raptor
</h4>

1. Enable GitHub SSH keys: 

> [!IMPORTANT]
> Needed to do only once

   Create an SSH key pair with the command below:

```bash
   ssh-keygen -t rsa -b 4096 -C "email@example.com"
```
This command will generate a key pair in `$HOME/.ssh` directory. Copy the value of the public SSH key (file with extension .pub) to the clipboard.
Log in to GitHub, navigate to your account settings, click on the SSH and GPG tab on the left side of the setting page, click Add Key to register the public SSH key with your account. Name the key and paste the copied value into the text field. Save your changes.

2. Clone the Raptor repository.

```
 git clone git@github.com:RapidSilicon/Raptor.git
```

3. Build the Raptor

```
  cd Raptor
  make
```
To check build is successful, run the below command:

```
make test/rs_gui
```
If the above command executes successfully, then Raptor is built and ready to use. 

**For Developers only**

* For debugging c/c++ code, build in Debug mode:

```  
  make debug
```

* After the build, if you want to install it to create a production build or need only binaries, then

> [!NOTE]
>     This will install Raptor in `/usr/local`, so you may need to add sudo. The location can be changed by passing `PREFIX=<any other location as install directory>`.

```
  make install 
  make test_install
```

> [!NOTE]
> During the build, all required submodules would be checked out by CMake, so no need to run `git submodule update --init --recursive` explicitly.

> [!NOTE]
> The Raptor_Tools submodule would be left uninitialized in all Raptor submodules.

### 2) ON COMPANY SERVER:

The instructions detail how to build Raptor from source code on company IT-managed Linux host. Make sure you get the server credentials and are able to log in to the server.

* SSH with X forwarding to compute server:
```
ssh -X <server name>
```
Possible server names at the time of writing this guide are:

* sw01
* sw02
* sw03

**Note:** Please consult with the IT manager to get the server name and working space name on which you are allowed to build Raptor.

* Load the module for the dependencies 
> [!IMPORTANT]
> Do this every time you open a new terminal.

```
module load fpga_tools/raptor/build_env
```

Now follow instructions described in [Steps to clone and build Raptor](#step-to-build-raptor)


### 3) NIGHTLY BUILD ACCESS:

The company build server automatically produces nightly builds of Raptor that can be used quickly for testing.  The instructions below detail how to connect to the automated build server and run Raptor in that environment.

* Log in to the server as per the instructions from the IT manager.

* SSH with X forwarding to compute server:
```
ssh -X <server name>
```
Possible servers name at writing of this guide are:

* fe1
* fe2
* sw01
* sw02
* sw03

> [!CAUTION]
> Please consult with IT-manager guy to get the name of server and working space name on which you are allowed to run nightly build..

* Load nightly built raptor:

```
module load fpga_tools/raptor/latest
```

* Launch raptor:
```
raptor
```

### Contributing to Raptor:

#### In Raptor repository

> [!IMPORTANT]
> This method is only for top Raptor level, for updating submodule see [below method](#updating-submodule).

  1. Clone the Raptor
  2. Switch to new branch:

  ```
  git checkout -b <branch name like EDA-968>
  ```
  3. Do your changes. 
  4. Do build to check your changes are not breaking any thing:

  ```
  make
  make test/batch_gen2 # this run tests. See Makefile for details of tests 
  ```

  5. Once you are sure then push your changes:

  ```
  git push origin --set-upstream <your branch name like EDA-968>
  ```

  6. Create PR on Raptor GitHub repository.

<h4 id="updating-submodule">
Updating Submodule in Raptor
</h4>

If you would like to record (commit) an update of the submodule then the procedure is as following:


> [!CAUTION]
> Raptor use CMake to control the checkout of submodules so avoid running `git submodule update --init --recurisve` directly to checkout the submodule as it will waste time and may result in unresolved complications.

  1. Clone the Raptor
  2. Switch to new branch:

  ```
  git checkout -b <branch name like EDA-968>
  ```
  3. Checkout submodules:

  ```
  make run-cmake-release
  ```
  4. Once all the submodules are checked out, then change the directory to the submodule you want to bump like Raptor_Tools or Backend and put its git pointer to the latest commit. For example, to update Backend:

  ```
  cd Backend && git checkout main && git pull origin && cd ..
  ```
  5. Use `git status` command to check your working tree status: what is added for commit, what is modified, what is untracked.

  6. Now in the Raptor root directory, update the Raptor git to record the new pointer of the submodule. 

> [!IMPORTANT]
> Always add/committing files/submodules explicitly. Do not use `git commit . -m "MESSAGE"` or `git add .` commands.

  For example:

  ```
  git add Backend
  git commit -m "Bump Backed or any message"
  ```
  7. It's a good idea to do the build before pushing.

  8. Once you are sure, push your changes:

  ```
  git push origin --set-upstream <your branch name like EDA-968>
  ```

  9. After the last command, you should be able to create a Pull Request in the Raptor repository.


