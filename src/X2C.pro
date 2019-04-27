#
# X2C.Qt5
#

TEMPLATE = app
TARGET = x2c

DEPENDPATH += .

QT += core
QT += xml
# QT -= gui

INCLUDEPATH += .

HEADERS += \
attribute.h \
attributeUse.h \
booleanType.h \
complexType.h \
dateType.h \
element.h \
elementUse.h \
generator.h \
listType.h \
normalizer.h \
numberType.h \
objects.h \
settings.h \
simpleType.h \
stringType.h \
type.h \
unionType.h \
utils.h

SOURCES += \
attribute.cpp \
attributeUse.cpp \
booleanType.cpp \
complexType.cpp \
dateType.cpp \
element.cpp \
elementUse.cpp \
generator.cpp \
listType.cpp \
main.cpp \
normalizer.cpp \
numberType.cpp \
objects.cpp \
settings.cpp \
simpleType.cpp \
stringType.cpp \
type.cpp \
unionType.cpp \
utils.cpp
