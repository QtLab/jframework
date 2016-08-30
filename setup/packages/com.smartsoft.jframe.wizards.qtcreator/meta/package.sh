#!/bin/sh

# ----- 设置变量 -----

# pwd
pwd="$(dirname $(readlink -f $0))"
#  设置框架变量
cd "$pwd/.."
jframe_dir="$PWD/jframework"
cd "$pwd"
# Qt SDK变量
qt_dir="$jframe_dir/bin/3rdpart/Qt"
# pack or unpack
if [ -z $1 ]; then
    pack_flag="true"
fi

echo ----------------------------------------
echo "$pwd"
echo ----------------------------------------
