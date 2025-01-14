#!/bin/zsh
cd $MILTON_PATH/build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -G "Ninja"
cmake --build .

$MILTON_PATH/build/src/Milton