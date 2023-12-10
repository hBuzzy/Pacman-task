QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    src/dragitem/dragitem.cpp \
    src/game.cpp \
    src/itemswidget/itemswidget.cpp \
    src/mainwindow/mainwindow.cpp \
    src/scene/scene.cpp \
    src/scene_items/itemsfactory.cpp \
    src/scene_items/sceneitem.cpp \

HEADERS += \
    src/dragitem/dragitem.h \
    src/game.h \
    src/itemswidget/itemswidget.h \
    src/mainwindow/mainwindow.h \
    src/scene/scene.h \
    src/scene_items/itemsfactory.h \
    src/scene_items/sceneitem.h \

FORMS += \
    src/mainwindow/mainwindow.ui

INCLUDEPATH += $$PWD/src/itemswidget

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
