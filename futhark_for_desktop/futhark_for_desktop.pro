QT += widgets
requires(qtConfig(filedialog))

HEADERS     = mainwindow.h \
              pieceslist.h \
              futharkwidget.h
RESOURCES   = futhark.qrc
SOURCES     = main.cpp \
              mainwindow.cpp \
		pieceslist.cpp \
              futharkwidget.cpp

QMAKE_PROJECT_NAME = futhark

# install
target.path = $$[QT_INSTALL_EXAMPLES]/futhark
INSTALLS += target
