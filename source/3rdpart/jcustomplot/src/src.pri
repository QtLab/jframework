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
    $$jcustomplot_headers \
    $$PWD/jchartview.h \
    $$PWD/jchart.h \
    $$PWD/jabstractchart.h \
    $$PWD/jabstractseries.h

SOURCES += \
    $$PWD/jcustomplot_global.cpp \
    $$PWD/jchartview.cpp \
    $$PWD/jchart.cpp \
    $$PWD/jabstractchart.cpp \
    $$PWD/jabstractseries.cpp

################################################
## framework
################################################

# for jcustomplot
framework_headers_jcustomplot.version = Versions
framework_headers_jcustomplot.path = Headers
framework_headers_jcustomplot.files = $$jcustomplot_headers

################################################
