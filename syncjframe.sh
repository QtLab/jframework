
#!/bin/sh

# --------------------
jframe_dir="$(dirname $(readlink -f $0))"
qt_dir="$QTDIR"
qt_major_ver=$1
qt_minor_ver=$2
qt_ver=$3
dbg_suffix=$4

#echo $jframe_dir
#echo $qt_dir
#echo $qt_ver
#echo $dbg_suffix

# --------------------

if [ ! -f "${jframe_dir}/bin/3rdpart/Qt/plugins/" ]; then
    mkdir -p "${jframe_dir}/bin/3rdpart/Qt/plugins/";
fi

if [ ! -f "${jframe_dir}/bin/jframe/" ]; then
    mkdir -p "${jframe_dir}/bin/jframe/";
fi

# --------------------

# --------------------

if [ $qt_major_ver -lt 5 ]; then
#######################################

qt_dir_old=$qt_dir
if [ $qt_ver = '4.8.7' ]; then
    qt_dir=/usr/lib/x86_64-linux-gnu
elif [ $qt_ver = '4.8.4' ]; then
    qt_dir=/usr/lib/
else
    exit 0
fi

cp -f "${qt_dir}/libQtCore.so" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/libQtCore.so.${qt_major_ver}" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/libQtCore.so.${qt_major_ver}.${qt_minor_ver}" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/libQtCore.so.${qt_ver}" "${jframe_dir}/bin/3rdpart/Qt/"

cp -f "${qt_dir}/libQtGui.so" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/libQtGui.so.${qt_major_ver}" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/libQtGui.so.${qt_major_ver}.${qt_minor_ver}" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/libQtGui.so.${qt_ver}" "${jframe_dir}/bin/3rdpart/Qt/"

cp -f "${qt_dir}/libQtXml.so" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/libQtXml.so.${qt_major_ver}" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/libQtXml.so.${qt_major_ver}.${qt_minor_ver}" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/libQtXml.so.${qt_ver}" "${jframe_dir}/bin/3rdpart/Qt/"

cp -f "${qt_dir}/libQtDBus.so" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/libQtDBus.so.${qt_major_ver}" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/libQtDBus.so.${qt_major_ver}.${qt_minor_ver}" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/libQtDBus.so.${qt_ver}" "${jframe_dir}/bin/3rdpart/Qt/"

qt_dir=$qt_dir/qt4

#######################################
else

#######################################
if [ $qt_ver = '5.7.0' ]; then
    cp -f "${qt_dir}/lib/libicudata.so.56" "${jframe_dir}/bin/3rdpart/Qt/"
    cp -f "${qt_dir}/lib/libicudata.so.56.1" "${jframe_dir}/bin/3rdpart/Qt/"
    cp -f "${qt_dir}/lib/libicui18n.so.56" "${jframe_dir}/bin/3rdpart/Qt/"
    cp -f "${qt_dir}/lib/libicui18n.so.56.1" "${jframe_dir}/bin/3rdpart/Qt/"
    cp -f "${qt_dir}/lib/libicuuc.so.56" "${jframe_dir}/bin/3rdpart/Qt/"
    cp -f "${qt_dir}/lib/libicuuc.so.56.1" "${jframe_dir}/bin/3rdpart/Qt/"
else
    cp -f "${qt_dir}/lib/libicui18n.so.${qt_major_ver}${qt_minor_ver}" "${jframe_dir}/bin/3rdpart/Qt/"
    cp -f "${qt_dir}/lib/libicuuc.so.${qt_major_ver}${qt_minor_ver}" "${jframe_dir}/bin/3rdpart/Qt/"
    cp -f "${qt_dir}/lib/libicudata.so.${qt_major_ver}${qt_minor_ver}" "${jframe_dir}/bin/3rdpart/Qt/"
fi
#######################################

cp -f "${qt_dir}/lib/libQt${qt_major_ver}Core.so" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_major_ver}Core.so.${qt_major_ver}" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_major_ver}Core.so.${qt_major_ver}.${qt_minor_ver}" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_major_ver}Core.so.${qt_ver}" "${jframe_dir}/bin/3rdpart/Qt/"

cp -f "${qt_dir}/lib/libQt${qt_major_ver}Gui.so" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_major_ver}Gui.so.${qt_major_ver}" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_major_ver}Gui.so.${qt_major_ver}.${qt_minor_ver}" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_major_ver}Gui.so.${qt_ver}" "${jframe_dir}/bin/3rdpart/Qt/"

cp -f "${qt_dir}/lib/libQt${qt_major_ver}Widgets.so" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_major_ver}Widgets.so.${qt_major_ver}" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_major_ver}Widgets.so.${qt_major_ver}.${qt_minor_ver}" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_major_ver}Widgets.so.${qt_ver}" "${jframe_dir}/bin/3rdpart/Qt/"

cp -f "${qt_dir}/lib/libQt${qt_major_ver}Concurrent.so" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_major_ver}Concurrent.so.${qt_major_ver}" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_major_ver}Concurrent.so.${qt_major_ver}.${qt_minor_ver}" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_major_ver}Concurrent.so.${qt_ver}" "${jframe_dir}/bin/3rdpart/Qt/"

cp -f "${qt_dir}/lib/libQt${qt_major_ver}Xml.so" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_major_ver}Xml.so.${qt_major_ver}" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_major_ver}Xml.so.${qt_major_ver}.${qt_minor_ver}" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_major_ver}Xml.so.${qt_ver}" "${jframe_dir}/bin/3rdpart/Qt/"

cp -f "${qt_dir}/lib/libQt${qt_major_ver}DBus.so" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_major_ver}DBus.so.${qt_major_ver}" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_major_ver}DBus.so.${qt_major_ver}.${qt_minor_ver}" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_major_ver}DBus.so.${qt_ver}" "${jframe_dir}/bin/3rdpart/Qt/"

cp -f "${qt_dir}/lib/libQt${qt_major_ver}XcbQpa.so" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_major_ver}XcbQpa.so.${qt_major_ver}" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_major_ver}XcbQpa.so.${qt_major_ver}.${qt_minor_ver}" "${jframe_dir}/bin/3rdpart/Qt/"
cp -f "${qt_dir}/lib/libQt${qt_major_ver}XcbQpa.so.${qt_ver}" "${jframe_dir}/bin/3rdpart/Qt/"

cp -f -r "${qt_dir}/plugins/platforms/" "${jframe_dir}/bin/3rdpart/Qt/plugins/"

fi

##
cp -f -r "${qt_dir}/plugins/iconengines/" "${jframe_dir}/bin/3rdpart/Qt/plugins/"
cp -f -r "${qt_dir}/plugins/imageformats/" "${jframe_dir}/bin/3rdpart/Qt/plugins/"
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

cp -f "${jframe_dir}/lib/jframe/libjframeworkdir.so" "${jframe_dir}/bin/"
cp -f "${jframe_dir}/lib/jframe/libjframeworkdir.so.1" "${jframe_dir}/bin/"
cp -f "${jframe_dir}/lib/jframe/libjframeworkdir.so.1.0" "${jframe_dir}/bin/"
cp -f "${jframe_dir}/lib/jframe/libjframeworkdir.so.1.0.0" "${jframe_dir}/bin/"

cp -f "${jframe_dir}/lib/jframe/libjframeworkenv.so" "${jframe_dir}/bin/jframe/"
cp -f "${jframe_dir}/lib/jframe/libjframeworkenv.so.1" "${jframe_dir}/bin/jframe/"
cp -f "${jframe_dir}/lib/jframe/libjframeworkenv.so.1.0" "${jframe_dir}/bin/jframe/"
cp -f "${jframe_dir}/lib/jframe/libjframeworkenv.so.1.0.0" "${jframe_dir}/bin/jframe/"

# --------------------

cp -f "${jframe_dir}/lib/jframe/libjframe_facade.so" "${jframe_dir}/bin/jframe/"
cp -f "${jframe_dir}/lib/jframe/libjframe_facade.so.1" "${jframe_dir}/bin/jframe/"
cp -f "${jframe_dir}/lib/jframe/libjframe_facade.so.1.0" "${jframe_dir}/bin/jframe/"
cp -f "${jframe_dir}/lib/jframe/libjframe_facade.so.1.0.0" "${jframe_dir}/bin/jframe/"

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
