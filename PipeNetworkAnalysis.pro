#-------------------------------------------------
#
# Project created by QtCreator 2012-04-25T03:28:15
#
#-------------------------------------------------

QT       += core gui

TARGET = PipeNetworkAnalysis
TEMPLATE = app


SOURCES += main.cpp\
    PipeEditorDialog.cpp \
    Fluid.cpp \
    PipeNetworkSystem.cpp \
    PipeNetworkGUI.cpp \
    Pipe.cpp \
    FlowNode.cpp \
    FlowBaseClass.cpp \
    Valve.cpp

HEADERS  += \
    PipeNetworkSystem.h \
    PipeEditorDialog.h \
    PipeNetworkGUI.h \
    Fluid.h \
    Pipe.h \
    FlowNode.h \
    FlowBaseClass.h \
    Constants.h \
    Valve.h

FORMS    += pipenetworkgui.ui \
    pipeeditordialog.ui

win32: LIBS += -L$$PWD/armadillo/lib_win32/ -llapack_win32_MT

INCLUDEPATH += $$PWD/armadillo/lib_win32
DEPENDPATH += $$PWD/armadillo/lib_win32

win32: PRE_TARGETDEPS += $$PWD/armadillo/lib_win32/lapack_win32_MT.lib

win32: LIBS += -L$$PWD/armadillo/lib_win32/ -lblas_win32_MT

INCLUDEPATH += $$PWD/armadillo/lib_win32
DEPENDPATH += $$PWD/armadillo/lib_win32

win32: PRE_TARGETDEPS += $$PWD/armadillo/lib_win32/blas_win32_MT.lib
