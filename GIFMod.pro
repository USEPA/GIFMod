CONFIG += debug_and_release debug_and_release_target
include ($$PWD/build-config.pri)
include ($$PWD/lapack.pri)

QT += core gui printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++14
TARGET = GIFMod 
TEMPLATE = app
win32:QMAKE_CXXFLAGS += /MP
QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS +=  -fopenmp
QMAKE_CFLAGS+=-pg
QMAKE_CXXFLAGS+=-pg
QMAKE_LFLAGS+=-pg
LIBS += -lgomp -lpthread

#defines
DEFINES += UNICODE GIFMOD
DEFINES += QT_version

DESTDIR = $${BUILD_DIR}
MOC_DIR = $${BUILD_DIR}/moc
OBJECTS_DIR = $${BUILD_DIR}/obj
UI_DIR = $${BUILD_DIR}/tmp-UI
RCC_DIR = $${BUILD_DIR}/tmp-RCC



LIBS+= -L$${BUILD_DIR} \
       -L$$[QT_INSTALL_BINS]

INCLUDEPATH += $$PWD/include \
               $$PWD/src/GUI  \
               $$PWD/src/duktape \
               $$PWD/src/dukglue \
               $$PWD/src/core \
               $$PWD/src/scripting

include ($$PWD/GIFMod.pri)

# post link actions to copy externals and resources

win32{

    COPY_EXTRA_FILES +=
    CONFIG(debug, debug|release) {
        COPY_EXTRA_FILES +=  $${LAPACK_LIB_DIR}/lapack_win64_MTd.dll \
                           $${LAPACK_LIB_DIR}/blas_win64_MTd.dll
    }
    CONFIG(release, debug|release) {
        COPY_EXTRA_FILES +=  $${LAPACK_LIB_DIR}/lapack_win64_MT.dll \
                           $${LAPACK_LIB_DIR}/blas_win64_MT.dll
    }
    COPY_EXTRA_FILES += $$PWD/src/resources/GIFModGUIPropList.csv
    COPY_EXTRA_FILES += $$PWD/src/resources/help.txt

    for(FILE,COPY_EXTRA_FILES){
                QMAKE_POST_LINK +=copy /Y "$$system_path($${FILE})" "$$system_path($$BUILD_DIR)" &&
    }


    QMAKE_POST_LINK += xcopy /Y /I "$$system_path($$PWD/src/GUI/Icons)" "$$system_path($${BUILD_DIR}/Icons)" &&
    QMAKE_POST_LINK += echo
}



# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


