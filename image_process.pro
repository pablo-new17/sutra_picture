QT       += core gui
#QT       += virtualkeyboard

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += console

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    MainWindow.cpp \
    MainWindow2.cpp \
    MainWindow3.cpp \
    MainWindow_1.cpp \
    PLable.cpp \
    main.cpp

HEADERS += \
    MainWindow.h \
    MainWindow2.h \
    MainWindow3.h \
    MainWindow_1.h \
    PLable.h

FORMS += \
    MainWindow.ui \
    MainWindow2.ui \
    MainWindow3.ui \
    MainWindow_1.ui

DISTFILES += \
        Release_Note.txt

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

CONFIG(release, debug|release): win32 {
	SOURCE_FILE = $${OUT_PWD}/release/$${TARGET}.exe
	SOURCE_FILE ~= s,/,\\,g
	RELEASE_NOTE = $$PWD/Release_Note.txt
	RELEASE_NOTE ~= s,/,\\,g

	TARGET_DIR = F:/workspace/projects/image_process_Installer/packages/tw.amtb.sutra_picuture/data/
	TARGET_DIR ~= s,/,\\,g

#	CONFIG(release, debug|release):message($$QMAKE_COPY $${SOURCE_FILE} $${TARGET_FILE})

	QMAKE_POST_LINK += $$QMAKE_COPY $$quote($${SOURCE_FILE}) $$quote($$TARGET_DIR) $$escape_expand(\\n\\t)
	QMAKE_POST_LINK += $$QMAKE_COPY $$quote($${RELEASE_NOTE}) $$quote($$TARGET_DIR) $$escape_expand(\\n\\t)
}
