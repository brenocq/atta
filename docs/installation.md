# Installation
To develop atta projects and have access to all its features, you will need to install atta on your machine. Not all features are be implemented on all operating systems right now, but we are working on it. If you want to use a feature that hasn't been implemented yet, please open a issue [here](https://github.com/brenocq/atta/issues).

If you just want to try projects developed with atta and don't care too much about runtime, try atta in your browser [here](https://atta.brenocq.com/build).

## Linux
Fist, you will need to install atta dependencies.

**Ubuntu:**
```bash
sudo apt-get install cmake xorg-dev openssl libssl-dev libboost-all-dev
```

**Arch:**
```bash
sudo pacman -Sy cmake glfw-x11 boost
```

Atta should build without errors when the compiller supports C++17. If you found any errors, please do not hesitate to [open an issue](https://github.com/brenocq/atta/issues).

```bash
git clone git@github.com:brenocq/atta.git
cd atta
mkdir build && cd build
cmake ..
make -j
sudo make install
atta_test
atta
```

## Windows
Fist, you will need to install atta dependencies.

```bash
choco install cmake
vcpkg install cpprestsdk cpprestsdk:x64-windows
```

Atta should build without errors when the compiller supports C++17. If you found any errors, please do not hesitate to [open an issue](https://github.com/brenocq/atta/issues).

```bash
git clone git@github.com:brenocq/atta.git
cd atta
mkdir build && cd build
cmake ..
make -j
./bin/atta_test
./bin/atta
```

## MacOS
Fist, you will need to install atta dependencies.

```bash
brew install cmake boost openssl 
```

Atta should build without errors when the compiller supports C++17. If you found any errors, please do not hesitate to [open an issue](https://github.com/brenocq/atta/issues).

```bash
git clone git@github.com:brenocq/atta.git
cd atta
mkdir build && cd build
cmake ..
make -j
./bin/atta_test
./bin/atta
```
