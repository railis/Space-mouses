HEADERS += mouse.h \
    object.h \
    window.h \
    player.h \
    projectile.h \
    customview.h \
    explosion.h
SOURCES += main.cpp \
    mouse.cpp \
    object.cpp \
    window.cpp \
    player.cpp \
    projectile.cpp \
    customview.cpp \
    explosion.cpp
RESOURCES += mice.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/graphicsview/collidingmice
sources.files = $$SOURCES \
    $$HEADERS \
    $$RESOURCES \
    $$FORMS \
    collidingmice.pro \
    images
sources.path = $$[QT_INSTALL_EXAMPLES]/graphicsview/collidingmice
INSTALLS += target \
    sources
symbian { 
    TARGET.UID3 = 0xA000A643
    include($$QT_SOURCE_TREE/examples/symbianpkgrules.pri)
}
FORMS += mainwindow.ui
