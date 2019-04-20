TEMPLATE=app
LIBS+=-L/usr/local/lib -L/home/datlucien/Documents/AP/CudaPBD/LuHuPBDLib/PBDLib/build -lgtest -lgtest_main -lPBDLib
#CONFIG+=c++11

SOURCES += src/main.cpp

OBJECTS_DIR+=obj/

TARGET= build/Tests

INCLUDEPATH+= ../PBDLib/include

#QMAKE_RPATHDIR+= $$PWD/../PBDLib

#unix:!macx: LIBS += -L$$PWD/../PBDLib/build/ -lPBDLib

#INCLUDEPATH += $$PWD/../PBDLib/include
#DEPENDPATH += $$PWD/../PBDLib/build
