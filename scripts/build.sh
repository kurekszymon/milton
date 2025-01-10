#!/usr/local/bin/zsh
cd $CPPAPP/build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -G "Ninja"
cmake --build .

$CPPAPP/build/src/Milton