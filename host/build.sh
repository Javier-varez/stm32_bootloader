#!/bin/bash

rm -rf dist
mkdir -p  dist/res
cp -R res/ dist/res/
pyinstaller --onefile QtBootloader.py
rm -rf build
