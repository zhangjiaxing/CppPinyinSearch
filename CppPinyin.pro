TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        phoneticconvert.cpp \
        pinyindict.cpp \
        pinyinmatcher.cpp \
        pinyintools.cpp

HEADERS += \
    phoneticconvert.h \
    pinyincommon.h \
    pinyindict.h \
    pinyinmatcher.h \
    pinyintools.h
