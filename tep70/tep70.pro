TEMPLATE = lib

QT -= gui
QT += xml

TARGET = tep70

DESTDIR = ../../modules/$$join(TARGET,,,)

LIBS += -L$$(RRS_DEV_ROOT)/bin -lCfgReader
LIBS += -L$$(RRS_DEV_ROOT)/bin -lphysics
LIBS += -L$$(RRS_DEV_ROOT)/bin -lvehicle
LIBS += -L$$(RRS_DEV_ROOT)/bin -ldevice
LIBS += -L$$(RRS_DEV_ROOT)/bin -lfilesystem

INCLUDEPATH += ./include
INCLUDEPATH += ../tep70-equipment/include
INCLUDEPATH += $$(RRS_DEV_ROOT)/sdk/include

HEADERS += $$files(./include/*.h)
HEADERS += $$files(../tep70-equipment/include/*.h)
SOURCES += $$files(./src/*.cpp)
SOURCES += $$files(../tep70-equipment/src/*.cpp)
