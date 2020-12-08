QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

 LIBS += -L "$$PWD/libs/" -lOpenCL

INCLUDEPATH += include/

SOURCES += \
    src/imagepanel.cpp \
    src/imagetab.cpp \
    src/imagemenu.cpp \
    src/imagewidget.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/nativeprocessor.cpp


HEADERS += \
    include/imagepanel.h \
    include/glwidget.h \
    include/imagemenu.h \
    include/imagetab.h \
    include/mainwindow.h \
    include/imagewidget.h \
    include/imagetab.h \
    include/nativeprocessor.h \
    include/ui_imagetab.h \
    include/ui_imagepanelform.h \
    include/ui_mainwindow.h


FORMS += \
    forms/imagePanelForm.ui \
    forms/imagetab.ui \
    forms/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
