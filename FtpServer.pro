TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        src/Server.cpp \
        src/Session.cpp

HEADERS += \
    src/Server.h \
    src/Session.h \
    src/SocketUntil.h

LIBS += -lpthread -lcrypt
