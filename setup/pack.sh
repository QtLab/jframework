#!/bin/sh

# ----- ���ñ��� -----

# pwd
pwd="$(dirname $(readlink -f $0))"
# ����·��
cd "$pwd/.."
jframe_dir="$PWD/jframework"
cd "$pwd"
# Qt SDK·��
qt_dir="$jframe_dir/bin/3rdpart/Qt"
# pack or unpack
if [ -z $1 ]; then
    pack_flag="true"
fi

echo ----------------------------------------
echo "$pwd"
echo ----------------------------------------

#echo $jframe_dir
#echo $qt_dir

# ----- fork package.sh -----

# ...

fork_packages()
{
    # ...
    cd $1

    # ...
    if [ -f "$1/package.sh" ]; then
        "$1/package.sh" "$jframe_dir" "$qt_dir" "$pack_flag"
    fi
    
    # ...
    for dirname in $(ls); do
        if [ -d "$dirname" ]; then
            fork_packages "$1/$dirname"
            cd $1
        fi
    done
}

# ...
fork_packages "$pwd/packages"

# ...
cd "$pwd"

# ...
if [ "$pack_flag" = "true" ]; then
    echo ...
    echo ...
    echo ======================================================
    echo Now it's packing to generate setup.run. Please wait...
    binarycreator --offline-only -c "$pwd/config/config.xml" -p "$pwd/packages" setup.run
    echo packing finished.
fi
