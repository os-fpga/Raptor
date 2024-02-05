# Raptor

## How to Build Raptor from Source Code

Build Locally on Your Machine:

Once your local machine is ready, please take into account the dependencies listed below. If you are unsure about the package installation status, go to the Raptor repository on GitHub by clicking [here](https://github.com/os-fpga/Raptor) and install the Raptor build dependencies by running the appropriate script:

- [Ubuntu dependencies](https://github.com/os-fpga/Raptor/blob/main/.github/workflows/install_ubuntu_dependencies_build.sh)
- [Centos dependencies](https://github.com/os-fpga/Raptor/blob/main/.github/workflows/install_centos_dependencies_build.sh)
- Not fully supported [MacOS dependencies](https://github.com/os-fpga/Raptor/blob/main/.github/workflows/install_macos_dependencies_build.sh)
- Not fully supported [Windows Msys2 dependencies](https://github.com/os-fpga/Raptor/blob/main/.github/workflows/main.yml)
- Not fully supported [Windows MSVC dependencies](https://github.com/os-fpga/Raptor/blob/main/.github/workflows/main.yml)

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

> [!CAUTION]
> Raptor use CMake to control the checkout of submodules so avoid running `git submodule update --init --recurisve` directly to checkout the submodule as it will waste time and may result in unresolved complications.

  1. Clone the Raptor
  2. Switch to new branch:

  ```
  git checkout -b <branch-name>
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

> [!CAUTION]
> Always add/committing files/submodules explicitly. Do not use `git commit . -m "MESSAGE"` or `git add .` commands.

  For example:

  ```
  git add Backend
  git commit -m "Bump Backend or any message"
  ```
  7. It's a good idea to do the build before pushing.

  8. Once you are sure, push your changes:

  ```
  git push origin --set-upstream <your branch name like EDA-968>
  ```

  9. After the last command, you should be able to create a Pull Request in the Raptor repository.


