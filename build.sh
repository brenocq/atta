#!/bin/bash
set -e

# Colors
BOLD="\033[1m"
RESET="\033[0m"
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
BLUE="\033[34m"
CYAN="\033[36m"
WHITE="\033[37m"
DIM="\033[2m"

info()    { echo -e "${BOLD}${BLUE}[atta]${RESET} $*"; }
success() { echo -e "${BOLD}${GREEN}[atta]${RESET} $*"; }
warn()    { echo -e "${BOLD}${YELLOW}[atta]${RESET} $*"; }
error()   { echo -e "${BOLD}${RED}[atta]${RESET} $*"; }
section() { echo -e "\n${BOLD}${CYAN}━━━ $* ${RESET}"; }

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
CMAKE_VULKAN="-DATTA_VULKAN_SUPPORT=ON"

printHelp()
{
    echo -e "${BOLD}Atta build script${RESET}"
    echo
    echo -e "Usage: ${CYAN}./build.sh${RESET} ${DIM}[option(s)]${RESET}"
    echo
    echo -e "${BOLD}Options:${RESET}"
    echo
    echo -e "  ${YELLOW}-h${RESET}, ${YELLOW}--help${RESET}"
    echo -e "        This help menu"
    echo
    echo -e "  ${YELLOW}-t${RESET}, ${YELLOW}--type${RESET} ${DIM}<option>${RESET}"
    echo -e "        Which type of build."
    echo -e "        Valid options are: ${CYAN}default${RESET}, ${CYAN}web${RESET}, ${CYAN}web_module${RESET}, ${CYAN}docs${RESET}"
    echo
    echo -e "  ${YELLOW}-d${RESET}, ${YELLOW}--debug${RESET}"
    echo -e "        Build with debug information."
    echo
    echo -e "  ${YELLOW}-D${RESET}, ${YELLOW}--debugger${RESET}"
    echo -e "        Run with a debugger (prefers LLDB, falls back to GDB)."
    echo
    echo -e "  ${YELLOW}-c${RESET}, ${YELLOW}--compiler${RESET} ${DIM}<name>${RESET}"
    echo -e "        Select the compiler."
    echo
    echo -e "  ${YELLOW}-j${RESET}, ${YELLOW}--jobs${RESET} ${DIM}<num_jobs>${RESET}"
    echo -e "        Set number of jobs to use when building."
    echo
    echo -e "  ${YELLOW}-s${RESET}, ${YELLOW}--static${RESET} ${DIM}<project_file>${RESET}"
    echo -e "        Build statically linked to a project."
    echo -e "        The file should be a valid ${DIM}.atta${RESET}"
    echo
    echo -e "  ${YELLOW}-r${RESET}, ${YELLOW}--run${RESET}"
    echo -e "        Run after build."
    echo
    echo -e "  ${YELLOW}-p${RESET}, ${YELLOW}--project${RESET} ${DIM}<project_file>${RESET}"
    echo -e "        Specify project to run."
    echo
    echo -e "  ${YELLOW}-i${RESET}, ${YELLOW}--install${RESET}"
    echo -e "        Install after build."
    echo
    echo -e "  ${YELLOW}--disable-vulkan${RESET}"
    echo -e "        Disable Vulkan support."
    exit
}

buildDefault()
{
    section "Building"
    cmake $CMAKE_BUILD_TYPE $CMAKE_COMPILER $CMAKE_ATTA_STATIC $CMAKE_VULKAN $SOURCE_PATH
    make -j $NUM_JOBS
    success "Build complete"

    if [[ "$INSTALL_AFTER" == "true" ]]; then
        section "Installing"
        sudo make install
        success "Install complete"
    fi

    if [[ "$RUN_AFTER" == "true" ]]; then
        section "Running"
        if [[ "$USE_DEBUGGER" == "true" ]]; then
            if command -v lldb &> /dev/null; then
                info "Using LLDB for debugging..."
                lldb --one-line "run" -- bin/atta -- $PROJECT_TO_RUN
            elif command -v gdb &> /dev/null; then
                info "Using GDB for debugging..."
                gdb -ex r --args bin/atta $PROJECT_TO_RUN
            else
                error "No debugger found (LLDB or GDB). Install one to continue."
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
    if ! command -v emcmake &> /dev/null
    then
        error "Emscripten is not installed, please follow the instructions at: ${WHITE}https://emscripten.org/docs/getting_started/downloads.html${RESET}"
        exit
    fi

    CMAKE_MODULE="-DATTA_WEB_BUILD_MODULE=OFF"
    if [[ "$BUILD_TYPE" == "web_module" ]]; then
        CMAKE_MODULE="-DATTA_WEB_BUILD_MODULE=ON"
    fi

    section "Building web"
    emcmake cmake $CMAKE_MODULE $CMAKE_BUILD_TYPE $CMAKE_ATTA_STATIC $SOURCE_PATH
    make -j $NUM_JOBS
    success "Web build complete"

    if [[ "$RUN_AFTER" == "true" ]]; then
        section "Running web"
        emrun bin/atta.html
    fi

    exit
}

buildDocs()
{
    section "Building docs"
    cmake -ATTA_BUILD_DOCS=ON -DATTA_BUILD_TESTS=OFF $SOURCE_PATH
    make -j $NUM_JOBS
    success "Docs build complete"
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
      error "Unknown option $1"
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
  error "Unknown build type ${BOLD}$BUILD_TYPE${RESET}"
  exit
  ;;
esac
