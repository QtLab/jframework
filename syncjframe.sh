#!/bin/bash

# --------------------
jframe_dir="$(dirname $(readlink -f $0))"
qt_dir="$QTDIR"
qt_ver=$1
dbg_suffix=$2

#echo $jframe_dir
#echo $qt_dir
#echo $qt_ver
#echo $dbg_suffix
# --------------------

if [ ${qt_ver} -lt 5 ]; then
    exit 0;
fi

# --------------------

if [ ! -f "${jframe_dir}/bin/3rdpart/" ]; then
    mkdir -p "${jframe_dir}/bin/3rdpart/";
fi

if [ ! -f "${jframe_dir}/bin/jframe/" ]; then
    mkdir -p "${jframe_dir}/bin/jframe/";
fi

# --------------------

cp -f "${qt_dir}/lib/libicui18n.so.54" "${jframe_dir}/bin/"
cp -f "${qt_dir}/lib/libicuuc.so.54" "${jframe_dir}/bin/"
cp -f "${qt_dir}/lib/libicudata.so.54" "${jframe_dir}/bin/"

cp -f "${qt_dir}/lib/libQt${qt_ver}Core.so" "${jframe_dir}/bin/"
cp -f "${qt_dir}/lib/libQt${qt_ver}Core.so.${qt_ver}" "${jframe_dir}/bin/"
cp -f "${qt_dir}/lib/libQt${qt_ver}Core.so.${qt_ver}.5" "${jframe_dir}/bin/"
cp -f "${qt_dir}/lib/libQt${qt_ver}Core.so.${qt_ver}.5.1" "${jframe_dir}/bin/"

cp -f "${qt_dir}/lib/libQt${qt_ver}Gui.so" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_ver}Gui.so.${qt_ver}" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_ver}Gui.so.${qt_ver}.5" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_ver}Gui.so.${qt_ver}.5.1" "${jframe_dir}/bin/3rdpart/Qt/"

cp -f "${qt_dir}/lib/libQt${qt_ver}Widgets.so" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_ver}Widgets.so.${qt_ver}" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_ver}Widgets.so.${qt_ver}.5" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_ver}Widgets.so.${qt_ver}.5.1" "${jframe_dir}/bin/3rdpart/Qt/"

cp -f "${qt_dir}/lib/libQt${qt_ver}Concurrent.so" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_ver}Concurrent.so.${qt_ver}" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_ver}Concurrent.so.${qt_ver}.5" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_ver}Concurrent.so.${qt_ver}.5.1" "${jframe_dir}/bin/3rdpart/Qt/"

cp -f "${qt_dir}/lib/libQt${qt_ver}Xml.so" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_ver}Xml.so.${qt_ver}" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_ver}Xml.so.${qt_ver}.5" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_ver}Xml.so.${qt_ver}.5.1" "${jframe_dir}/bin/3rdpart/Qt/"

cp -f -r "${qt_dir}/plugins/iconengines/" "${jframe_dir}/bin/3rdpart/Qt/plugins/"
cp -f -r "${qt_dir}/plugins/imageformats/" "${jframe_dir}/bin/3rdpart/Qt/plugins/"
cp -f -r "${qt_dir}/plugins/platforms/" "${jframe_dir}/bin/3rdpart/Qt/plugins/"
cp -f -r "${qt_dir}/plugins/sqldrivers/" "${jframe_dir}/bin/3rdpart/Qt/plugins/"

# --------------------

cp -f "${jframe_dir}/lib/3rdpart/libjwt.so" "${jframe_dir}/bin/3rdpart/"
cp -f "${jframe_dir}/lib/3rdpart/libjwt.so.1" "${jframe_dir}/bin/3rdpart/"
cp -f "${jframe_dir}/lib/3rdpart/libjwt.so.1.0" "${jframe_dir}/bin/3rdpart/"
cp -f "${jframe_dir}/lib/3rdpart/libjwt.so.1.0.0" "${jframe_dir}/bin/3rdpart/"

cp -f "${jframe_dir}/lib/3rdpart/liblog4cpp.so" "${jframe_dir}/bin/3rdpart/"
cp -f "${jframe_dir}/lib/3rdpart/liblog4cpp.so.1" "${jframe_dir}/bin/3rdpart/"
cp -f "${jframe_dir}/lib/3rdpart/liblog4cpp.so.1.0" "${jframe_dir}/bin/3rdpart/"
cp -f "${jframe_dir}/lib/3rdpart/liblog4cpp.so.1.0.0" "${jframe_dir}/bin/3rdpart/"

cp -f "${jframe_dir}/lib/3rdpart/libqtribbon.so" "${jframe_dir}/bin/3rdpart/"
cp -f "${jframe_dir}/lib/3rdpart/libqtribbon.so.1" "${jframe_dir}/bin/3rdpart/"
cp -f "${jframe_dir}/lib/3rdpart/libqtribbon.so.1.0" "${jframe_dir}/bin/3rdpart/"
cp -f "${jframe_dir}/lib/3rdpart/libqtribbon.so.1.0.0" "${jframe_dir}/bin/3rdpart/"

cp -f "${jframe_dir}/lib/3rdpart/libqtwinmigrate.so" "${jframe_dir}/bin/3rdpart/"
cp -f "${jframe_dir}/lib/3rdpart/libqtwinmigrate.so.1" "${jframe_dir}/bin/3rdpart/"
cp -f "${jframe_dir}/lib/3rdpart/libqtwinmigrate.so.1.0" "${jframe_dir}/bin/3rdpart/"
cp -f "${jframe_dir}/lib/3rdpart/libqtwinmigrate.so.1.0.0" "${jframe_dir}/bin/3rdpart/"

cp -f "${jframe_dir}/lib/3rdpart/libtinyxml.so" "${jframe_dir}/bin/3rdpart/"
cp -f "${jframe_dir}/lib/3rdpart/libtinyxml.so.1" "${jframe_dir}/bin/3rdpart/"
cp -f "${jframe_dir}/lib/3rdpart/libtinyxml.so.1.0" "${jframe_dir}/bin/3rdpart/"
cp -f "${jframe_dir}/lib/3rdpart/libtinyxml.so.1.0.0" "${jframe_dir}/bin/3rdpart/"

# --------------------

cp -f "${jframe_dir}/lib/jframe/libjframe_facade.so" "${jframe_dir}/bin/"
cp -f "${jframe_dir}/lib/jframe/libjframe_facade.so.1" "${jframe_dir}/bin/"
cp -f "${jframe_dir}/lib/jframe/libjframe_facade.so.1.0" "${jframe_dir}/bin/"
cp -f "${jframe_dir}/lib/jframe/libjframe_facade.so.1.0.0" "${jframe_dir}/bin/"

cp -f "${jframe_dir}/lib/jframe/libjframe_factory.so" "${jframe_dir}/bin/jframe/"
cp -f "${jframe_dir}/lib/jframe/libjframe_factory.so.1" "${jframe_dir}/bin/jframe/"
cp -f "${jframe_dir}/lib/jframe/libjframe_factory.so.1.0" "${jframe_dir}/bin/jframe/"
cp -f "${jframe_dir}/lib/jframe/libjframe_factory.so.1.0.0" "${jframe_dir}/bin/jframe/"

cp -f "${jframe_dir}/lib/jframe/libjframe_kernel.so" "${jframe_dir}/bin/jframe/"
cp -f "${jframe_dir}/lib/jframe/libjframe_kernel.so.1" "${jframe_dir}/bin/jframe/"
cp -f "${jframe_dir}/lib/jframe/libjframe_kernel.so.1.0" "${jframe_dir}/bin/jframe/"
cp -f "${jframe_dir}/lib/jframe/libjframe_kernel.so.1.0.0" "${jframe_dir}/bin/jframe/"

# --------------------

exit 0

#end
