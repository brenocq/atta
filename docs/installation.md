# Installation
To develop atta projects and have access to all its features, you will need to install atta on your machine. Not all features are be implemented on all operating systems right now, but we are working on it. If you want to use a feature that hasn't been implemented yet, please open a issue [here](https://github.com/brenocq/atta/issues).

If you just want to try projects developed with atta and don't care too much about runtime, try atta in your browser [here](https://atta.brenocq.com/build).

## Linux
Fist, you will need to install atta dependencies.

**Ubuntu:**
```bash
$ sudo apt-get install ninja-build cmake xorg-dev
```

**Arch:**
```bash
$ sudo pacman -Sy ninja cmake glfw-x11
```

After installing the dependencies, be sure to have your **github ssh key properly configured**, you will be asked to insert the password many times. 
Be sure to **clone with the `--recursive` flag**. If you accidentally cloned without this flag, please run `git submodule update --init --recursive` to clone the submodules.

Atta should build without errors when the compiller supports C++17. If you found any errors, please do not hesitate to [open an issue]((https://github.com/brenocq/atta/issues).

```bash
$ git clone --recursive git@github.com:brenocq/atta.git
$ cd atta
$ mkdir build && cd build
$ cmake ..
$ make -j
$ sudo make install
$ atta_test
$ atta
```

## Windows
Fist, you will need to install atta dependencies.

```
choco install ninja cmake
```

After installing the dependencies, be sure to have your **github ssh key properly configured**, you will be asked to insert the password many times. 
Be sure to **clone with the `--recursive` flag**. If you accidentally cloned without this flag, please run `git submodule update --init --recursive` to clone the submodules.

Atta should build without errors when the compiller supports C++17. If you found any errors, please do not hesitate to [open an issue]((https://github.com/brenocq/atta/issues).

```bash
$ git clone --recursive git@github.com:brenocq/atta.git
$ cd atta
$ mkdir build && cd build
$ cmake ..
$ make -j
$ ./bin/atta_test
$ ./bin/atta
```


## MacOS
Fist, you will need to install atta dependencies.

```
brew install ninja cmake
```

After installing the dependencies, be sure to have your **github ssh key properly configured**, you will be asked to insert the password many times. 
Be sure to **clone with the `--recursive` flag**. If you accidentally cloned without this flag, please run `git submodule update --init --recursive` to clone the submodules.

Atta should build without errors when the compiller supports C++17. If you found any errors, please do not hesitate to [open an issue]((https://github.com/brenocq/atta/issues).

```bash
$ git clone --recursive git@github.com:brenocq/atta.git
$ cd atta
$ mkdir build && cd build
$ cmake ..
$ make -j
$ ./bin/atta_test
$ ./bin/atta
```
