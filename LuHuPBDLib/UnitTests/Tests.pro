TEMPLATE=app


CONFIG+=c++11

#LIBS+= -L/home/s4906706/Documents/AP/Projects/CudaPBD/LuHuPBDLib/PBDLib/build -lLuHuPBD



#message(system($$PWD/../PBDLib/build))


SOURCES += src/main.cpp

OBJECTS_DIR+=obj/

TARGET= build/Tests

INCLUDEPATH+= ../PBDLib/include

LIBS+= -L/home/datlucien/Documents/AP/CudaPBD/LuHuPBDLib/PBDLib -lLuHuPBD
LIBS+= -L/usr/local/lib  -lgtest -lgtest_main

#QMAKE_RPATHDIR+= $$PWD/../PBDLib

#unix:!macx: LIBS += -L$$PWD/../PBDLib/build/ -lPBDLib

#INCLUDEPATH += $$PWD/../PBDLib/include
#DEPENDPATH += $$PWD/../PBDLib/build
