QT           += widgets
QT +=opengl

HEADERS       = \
                mainwindow.h \
    RootViewController.h \
    AbstractView.h \
    TreeView.h \
    StandardItemModelEx.h \
    face.h \
    camera.h \
    model.h \
    mywidget.h \
    openglwidget.h
SOURCES       = \
                main.cpp \
                mainwindow.cpp \
    RootViewController.cpp \
    AbstractView.cpp \
    TreeView.cpp \
    face.cpp \
    camera.cpp \
    model.cpp \
    mywidget.cpp \
    openglwidget.cpp



# install
target.path = $$[QT_INSTALL_EXAMPLES]/opengl/hellogl2
INSTALLS += target

RESOURCES += \
    icon.qrc
