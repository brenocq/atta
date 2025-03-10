#!/bin/bash
set -e

SCRIPT_PATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
SOURCE_PATH="$SCRIPT_PATH"
BUILD_PATH="$SOURCE_PATH/build"
CMAKE_BUILD_TYPE="-DCMAKE_BUILD_TYPE=Release"
CMAKE_COMPILER=""
BUILD_NAME_PREFIX=""
BUILD_NAME="release"
BUILD_NAME_SUFIX=""
CMAKE_ATTA_STATIC=""
BUILD_TYPE="default"
RUN_AFTER="false"
PROJECT_TO_RUN=""
INSTALL_AFTER="false"
NUM_JOBS=""
CMAKE_VULKAN=""

printHelp()
{
    echo "Atta build script"
    echo
    echo "Usage: ./build.sh [option(s)]"
    echo
    echo "Options:"
    echo
    echo "-h or --help"
    echo "        This help menu"
    echo
    echo "-t or --type <option>"
    echo "        Which type of build."
    echo "        Valid options are: default, web, web_module, docs"
    echo
    echo "-d or --debug"
    echo "        Build with debug information."
    echo
    echo "-D or --debugger"
    echo "        Run with a debugger (prefers LLDB, falls back to GDB)."
    echo
    echo "-c or --compiler <name>"
    echo "        Select the compiler."
    echo
    echo "-j or --jobs <num_jobs>"
    echo "        Set number of jobs to use when building."
    echo
    echo "-s or --static <project_file>"
    echo "        Build statically linked to a project."
    echo "        The file should be a valid .atta"
    echo
    echo "-r or --run"
    echo "        Run after build."
    echo
    echo "-p or --project <project_file>"
    echo "        Specify project to run."
    echo
    echo "-i or --install"
    echo "        Install after build."
    echo
    echo "--disable-vulkan"
    echo "        Disable Vulkan support."
    exit
}

buildDefault()
{
    echo "---------- Building ----------"
    # Build
    cmake $CMAKE_BUILD_TYPE $CMAKE_COMPILER $CMAKE_ATTA_STATIC $CMAKE_VULKAN $SOURCE_PATH
    make -j $NUM_JOBS

    # Install
    if [[ "$INSTALL_AFTER" == "true" ]]; then
        echo "---------- Installing ----------"
       sudo make install
    fi

    # Run
    if [[ "$RUN_AFTER" == "true" ]]; then
        echo "---------- Running ----------"
        if [[ "$USE_DEBUGGER" == "true" ]]; then
            # Detect available debugger (prefer LLDB)
            if command -v lldb &> /dev/null; then
                echo "Using LLDB for debugging..."
                lldb --one-line "run" -- bin/atta -- $PROJECT_TO_RUN
            elif command -v gdb &> /dev/null; then
                echo "Using GDB for debugging..."
                gdb -ex r --args bin/atta $PROJECT_TO_RUN
            else
                echo -e "\033[1m\033[31m[Error] \033[0m\033[31mNo debugger found (LLDB or GDB). Install one to continue."
                exit 1
            fi
        else
            bin/atta $PROJECT_TO_RUN
        fi
    fi

    exit
}

buildWeb()
{
    # Check if emscripten is installed
    if ! command -v emcmake &> /dev/null
    then
        echo -e "\033[1m\033[31m[Error] \033[0m\033[31mEmscripten is not installed, please follow the instruction here: \033[37mhttps://emscripten.org/docs/getting_started/downloads.html"
        exit
    fi

    CMAKE_MODULE="-DATTA_WEB_BUILD_MODULE=OFF"
    if [[ "$BUILD_TYPE" == "web_module" ]]; then
        CMAKE_MODULE="-DATTA_WEB_BUILD_MODULE=ON"
    fi

    # Build
    echo "---------- Building web ----------"
    emcmake cmake $CMAKE_MODULE $CMAKE_BUILD_TYPE $CMAKE_ATTA_STATIC $SOURCE_PATH
    make -j $NUM_JOBS

    # Run
    if [[ "$RUN_AFTER" == "true" ]]; then
        echo "---------- Running web ----------"
        emrun bin/atta.html
    fi

    exit
}

buildDocs()
{
    echo "---------- Building docs ----------"
    cmake -ATTA_BUILD_DOCS=ON -DATTA_BUILD_TESTS=OFF $SOURCE_PATH
    make -j $NUM_JOBS
    exit
}

# Parse arguments
while [[ $# -gt 0 ]]; do
  case $1 in
    -h|--help)
      printHelp
      ;;
    -d|--debug)
      CMAKE_BUILD_TYPE="-DCMAKE_BUILD_TYPE=Debug"
      BUILD_NAME="debug"
      shift # past argument
      ;;
    -D|--debugger)
      USE_DEBUGGER="true"
      RUN_AFTER="true"
      shift # past argument
      ;;
    -r|--run)
      RUN_AFTER="true"
      shift # past argument
      ;;
    -j|--jobs)
      NUM_JOBS="$2"
      shift # past argument
      shift # past value
      ;;
    -p|--project)
      PROJECT_TO_RUN="$2"
      shift # past argument
      shift # past value
      ;;
    -i|--install)
      INSTALL_AFTER="true"
      shift # past argument
      ;;
      -c|--compiler)
      CMAKE_COMPILER="-DCMAKE_CXX_COMPILER=$2"
      shift # past argument
      shift # past vlaue
      ;;
    -t|--type)
      BUILD_TYPE="$2"
      if [[ "$BUILD_TYPE" != "default" ]]; then
          BUILD_NAME_PREFIX="$BUILD_TYPE-"
      fi
      shift # past argument
      shift # past value
      ;;
    -s|--static)
      CMAKE_ATTA_STATIC="-DATTA_STATIC_PROJECT_FILE=$2"
      BUILD_NAME_SUFIX="-static"
      shift # past argument
      shift # past value
      ;;
    --disable-vulkan)
      CMAKE_VULKAN="-DATTA_VULKAN_SUPPORT=OFF"
      shift
      ;;
    -*|--*)
      echo "Unknown option $1"
      exit
      ;;
  esac
done


# Change to build directory
BUILD_PATH="$BUILD_PATH/$BUILD_NAME_PREFIX$BUILD_NAME$BUILD_NAME_SUFIX"
mkdir -p $BUILD_PATH && cd $BUILD_PATH

# Build
case $BUILD_TYPE in
default)
  buildDefault
  ;;
web|web_module)
  buildWeb
  ;;
docs)
  buildDocs
  ;;
*)
  echo "Unknown build type $BUILD_TYPE"
  exit
  ;;
esac
