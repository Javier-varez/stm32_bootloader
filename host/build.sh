#!/bin/bash

rm -rf dist
pyinstaller QtBootloader.spec
rm -rf build
