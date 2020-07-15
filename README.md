# MySimulator
Simulator core made with Vulkan and ray tracing

## Installation (Linux)
### Updating g++
This simulator is using some libraries that were experimental in old version of g++. We can or change all #include<xxx> with erros to #include<expermental/xxx>, or update the g++.
We can use g++ v8.2 or later to compile without errors.
``` shell
g++ --version
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install gcc-8 g++-8
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-8 40
g++ --version
```


### Install Vulkan SDK
First we need to download the vulkan SDK, I'm using the release 1.2.135.0.
You can download the vulkan SDK [here](https://vulkan.lunarg.com/sdk/home).

After downlading:
``` shell
tar -xzf vulkansdk-linux-x86_64-xxx.tar.gz
cd <xxx>
```

### Install GLFW
Now we need to install the GLFW to create windows.

```shell
sudo apt-get install libglfw3-dev
```
If you prefer, you build manually from the [official website](https://www.glfw.org/).

### Install GLM
Now we'll have to download the GLM library to perform linear algebra.

```shell
sudo apt install libglm-dev
```

### Change MakeFile
The last pass is to change the vulkan SDK path in the makefile:

```
# Open Makefile
VULKAN_SDK_PATH = <path>/<xxx>/x86_64
```
