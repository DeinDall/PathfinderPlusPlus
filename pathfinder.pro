CONFIG -= qt
CONFIG += c++11

QMAKE_LFLAGS += -static

CONFIG(debug) {
    LIBS += -lsfml-graphics-s-d -lsfml-window-s-d -lsfml-system-s-d -lsfml-main-d
} else {
    LIBS += -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lsfml-main
}

DEFINES += SFML_STATIC

TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/direction.cpp \
    src/map.cpp \
    src/pathfinder.cpp \
    src/console.cpp
	
HEADERS += \
    src/map.h \
    src/direction.h \
    src/pathfinder.h \
    src/console.h
