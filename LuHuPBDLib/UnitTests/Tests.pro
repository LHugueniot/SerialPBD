TEMPLATE=app

CONFIG+=c++11

SOURCES += src/*.cpp \

OBJECTS_DIR+=obj/

TARGET= build/Tests

INCLUDEPATH+= ../PBDLib/include

QMAKE_RPATHDIR+= $$PWD/../PBDLib/build

LIBS+= -L$$PWD/../PBDLib/build -lLuHuPBD

LIBS+= -L/usr/local/lib  -lgtest -lgtest_main -lassimp
