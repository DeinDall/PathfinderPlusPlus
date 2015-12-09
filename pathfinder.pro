CONFIG -= qt
CONFIG += c++11

QMAKE_LFLAGS += -static

LIBS += -lcomdlg32

CONFIG(debug) {
    LIBS += -lsfml-graphics-s-d -lsfml-window-s-d -lsfml-system-s-d -lsfml-main-d
} else {
    LIBS += -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lsfml-main
}

DEFINES += SFML_STATIC

TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/interface/stk_fileopenwrapper.cpp \
    src/direction.cpp \
    src/map.cpp \
    src/pathfinder.cpp \
    src/mousedrawer.cpp \
    src/interface/button.cpp \
    src/interface/buttons.cpp \
    src/interface/pathinfo.cpp \
    src/flag.cpp \
    src/interface/console.cpp

HEADERS += \
    src/map.h \
    src/perso.h \
    src/direction.h \
    src/pathfinder.h \
    src/interface/button.h \
    src/interface/console.h \
    src/interface/buttons.h \
    src/mousedrawer.h \
    src/interface/stk_fileopenwrapper.h \
    src/interface/pathinfo.h \
    src/flag.h
