TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        phoneticconvert.cpp \
        pinyindict.cpp \
        pinyintools.cpp

HEADERS += \
    phoneticconvert.h \
    pinyin_common.h \
    pinyindict.h \
    pinyintools.h


CONFIG += link_pkgconfig
PKGCONFIG += Qt5X11Extras
