QT       += core gui
QT       += core gui sql
QT       += network
QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chatSession.cpp \
    chatserver.cpp \
    chatwindow.cpp \
    contactdialog.cpp \
    editprofiledialog.cpp \
    main.cpp \
    mainwindow.cpp \
    message.cpp \
    networkManager.cpp \
    registerdialog.cpp \
    user.cpp \
    userManager.cpp \
    userdialog.cpp

HEADERS += \
    chatSession.h \
    chatserver.h \
    chatwindow.h \
    contactdialog.h \
    editprofiledialog.h \
    mainwindow.h \
    message.h \
    networkManager.h \
    registerdialog.h \
    user.h \
    userManager.h \
    userdialog.h

FORMS += \
    chatwindow.ui \
    contactdialog.ui \
    editprofiledialog.ui \
    mainwindow.ui \
    registerdialog.ui \
    userdialog.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ../fotki.qrc \
    ../images.qrc \
    logo.qrc \
    logoTEX.qrc \
    obraz.qrc
