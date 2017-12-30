#-------------------------------------------------
#
# Project created by QtCreator 2017-12-16T13:24:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pictureswarm
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    vehicle.cpp \
    vhelper.cpp \
    mhelper.cpp \
    chelper.cpp \
    perlinnoise.cpp \
    particle.cpp

HEADERS += \
        mainwindow.h \
    vehicle.h \
    vhelper.h \
    mhelper.h \
    chelper.h \
    perlinnoise.h \
    particle.h \
    spline_library/splines/cubic_hermite_spline.h \
    spline_library/splines/generic_b_spline.h \
    spline_library/splines/natural_spline.h \
    spline_library/splines/quintic_hermite_spline.h \
    spline_library/splines/uniform_cr_spline.h \
    spline_library/splines/uniform_cubic_bspline.h \
    spline_library/utils/arclength.h \
    spline_library/utils/calculus.h \
    spline_library/utils/linearalgebra.h \
    spline_library/utils/nanoflann.hpp \
    spline_library/utils/spline_common.h \
    spline_library/utils/splineinverter.h \
    spline_library/utils/splinesample_adaptor.h \
    spline_library/spline.h \
    spline_library/vector.h

FORMS += \
        mainwindow.ui
