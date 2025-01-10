#!/usr/local/bin/zsh
cd $CPPAPP
rm -rf build/*
conan install . --build=missing --output-folder=conan

