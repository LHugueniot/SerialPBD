TEMPLATE=app
LIBS+=-L/home/s4906706/Documents/AP/Projects/CudaPBD/LuHuPBDLib/PBDLib/build -lLuHuPBD
LIBS+=-L/usr/local/lib  -lgtest -lgtest_main  #-L../../PBDLib/build -L/home/datlucien/Documents/AP/CudaPBD/LuHuPBDLib/PBDLib/build

#message(system($$PWD/../PBDLib/build))

#CONFIG+=c++11

SOURCES += src/main.cpp

OBJECTS_DIR+=obj/

TARGET= build/Tests

INCLUDEPATH+= ../PBDLib/include

#QMAKE_RPATHDIR+= $$PWD/../PBDLib

#unix:!macx: LIBS += -L$$PWD/../PBDLib/build/ -lPBDLib

#INCLUDEPATH += $$PWD/../PBDLib/include
#DEPENDPATH += $$PWD/../PBDLib/build
