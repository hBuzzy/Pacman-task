QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    pacmanarea/dragitem/coindragitem.cpp \
    pacmanarea/dragitem/dragitemfabric.cpp \
    pacmanarea/dragitem/walldragitem.cpp \
    pacmanarea/pacman/pacmanitem.cpp \
    pacmanarea\dragitem\abstractdragitem.cpp \
    pacmanarea\droppablearea\droppablearea.cpp \
    main.cpp \
    mainwindow.cpp \
    pacmanarea\pacmanarea.cpp

HEADERS += \
    pacmanarea/dragitem/coindragitem.h \
    pacmanarea/dragitem/dragitemfabric.h \
    pacmanarea/dragitem/walldragitem.h \
    pacmanarea/pacman/pacmanitem.h \
    pacmanarea\dragitem\abstractdragitem.h \
    pacmanarea\droppablearea\droppablearea.h \
    mainwindow.h \
    pacmanarea\pacmanarea.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
