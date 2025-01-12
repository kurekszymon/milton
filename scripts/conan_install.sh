#!/usr/local/bin/zsh
cd $MILTON_PATH
rm -rf build/*
conan install . --build=missing --output-folder=conan

