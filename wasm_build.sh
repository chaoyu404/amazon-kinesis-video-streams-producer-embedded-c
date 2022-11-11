#!/bin/bash
# variable' default value
WASM_BUILD_DIR="wasm_build"
DEFAULT_TOOLCHAIN_PATH=/opt/wasi-sdk

# set Tool Path
[ -z $TOOLCHAIN_PATH ] && \
export TOOLCHAIN_PATH=${DEFAULT_TOOLCHAIN_PATH}
echo "The Tool Path is" $TOOLCHAIN_PATH

# make the building directory
[ -d $WASM_BUILD_DIR ] && rm $WASM_BUILD_DIR -rf
mkdir $WASM_BUILD_DIR
cd $WASM_BUILD_DIR


: <<'END'
if [ ! -d $WASM_BUILD_DIR ]
then
    mkdir $WASM_BUILD_DIR;
    cd $WASM_BUILD_DIR;
    cmake ..  -DCMAKE_TOOLCHAIN_FILE=wasm_toolchain.txt
else
    cd $WASM_BUILD_DIR
fi
END


cmake ..  -DCMAKE_TOOLCHAIN_FILE=wasm_toolchain.txt -DCMAKE_BUILD_TYPE=Debug
make -j 4 --trace