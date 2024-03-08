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
> Needed to do only once. You can skip it if you have no intention of contributing back. 

   Create an SSH key pair with the command below:

```bash
   ssh-keygen -t rsa -b 4096 -C "email@example.com"
```
This command will generate a key pair in the `$HOME/.ssh` directory. Copy the value of the public SSH key (file with extension .pub) to the clipboard.
Log in to GitHub, navigate to your account settings, click on the SSH and GPG tab on the left side of the settings page, and click Add Key to register the public SSH key with your account. Name the key and paste the copied value into the text field. Save your changes.

2. Clone the Raptor repository.

```
 git clone git@github.com:os-fpga/Raptor.git
```

> [!CAUTION]
> Raptor has many submodules. Raptor uses CMake to control the checkout of submodules so avoid running `git submodule update --init --recursive` directly to checkout the submodule as it will waste time and may result in unresolved complications. The Raptor_Tools submodule would be left uninitialized in all Raptor submodules.

3. Build the Raptor

> [!TIP]
> If you are doing Raptor build in WSL or Docker environment then turn off Monaco Editor by adding the flag `MONACO_EDITOR=0` as it is not supported in these environments.

```
  cd Raptor
  make
```
To check build is successful, run the below command:

```
make test/raptor_gui
or
make test/raptor_batch

```
If the above command executes successfully, then Raptor is built and ready to use. 

4. [Optional] After the build, if you want to install it then

> [!NOTE]
> This will install Raptor in `/usr/local`, so you may need to add sudo. The location can be changed by passing `PREFIX=<any other location as install directory>`.

```
  make install 
  make test_install
```

**For Developers only**

* For debugging c/c++ code, build in Debug mode:

```  
  make debug
```

## Contributing to Raptor

  1. Fork Raptor
  2. Clone the Raptor
  3. Switch to a new branch:

  ```
  git checkout -b <branch-name>
  ```
  4. Do your changes, do the build as mentioned above, test your changes, push back to your fork, and create a PR

### Updating Submodule at Raptor level

  1. Clone Raptor
  2. Checkout submodule using the below command:
      
  ```
  make run-cmake-release
  ```
  3. Once all the submodules are checked out, then change the directory to the submodule you want to bump like Raptor_Tools or Backend, and put its git pointer to the latest commit. For example, to update Backend:

  ```
  cd Backend && git checkout main && git pull origin && cd ..
  ```
  4. Use the `git status` command to check your working tree status. What is added for commit, what is modified, and what is untracked?

  5. Now in the Raptor root directory, update the Raptor git to record the new pointer of the submodule. 

> [!CAUTION]
> Always add/commit files/submodules explicitly. Do not use `git commit. -m "MESSAGE"` or `git add .` commands.

  For example:

  ```
  git add Backend
  git commit -m "Bump Backend or any message"
  ```
  6. It's a good idea to do the build before pushing.

  7. Once you are sure, push your changes:

  ```
  git push origin --set-upstream <your branch name>
  ```

  8. After push, you should be able to create a Pull Request in the Raptor repository.


