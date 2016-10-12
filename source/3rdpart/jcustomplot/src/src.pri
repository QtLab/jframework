#-------------------------------------------------
#
# Project created by QtCreator 2015-08-01T00:00:11
#
#-------------------------------------------------

################################################################
# source fiels
################################################################

################################################
## jcustomplot - sources

jcustomplot_headers += \
    $$PWD/jcustomplot_global.h \
    $$PWD/jcustomplot_inc.h

HEADERS += \
    $$jcustomplot_headers

SOURCES += \
    $$PWD/jcustomplot_global.cpp

################################################
## framework
################################################

# for jcustomplot
framework_headers_jcustomplot.version = Versions
framework_headers_jcustomplot.path = Headers
framework_headers_jcustomplot.files = $$jcustomplot_headers

################################################
