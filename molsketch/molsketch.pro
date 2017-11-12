include(../settings.pri)

TEMPLATE = app
RESOURCES += *.qrc
FORMS += *.ui
HEADERS += *.h
SOURCES += *.cpp
LIBS += -L../lib -lmolsketch$$qtVersionSuffix
TARGET = molsketch$$qtVersionSuffix
DESTDIR = ../bin
target.path=$${MSK_INSTALL_BINS}
INSTALLS += target
QT += network

unix {
  INSTALLS += icon pixmap mimeicon mimetype desktop appdata

  icon.files = images/molsketch.svg
  icon.path = $$MSK_PREFIX/share/icons/hicolor/scalable/apps

  pixmap.files = images/molsketch.xpm
  pixmap.path = $$MSK_PREFIX/share/pixmaps

  mimeicon.files = images/application-x-molsketch.svg
  mimeicon.path = $$MSK_PREFIX/share/icons/hicolor/scalable/mimetypes

  mimetype.files = molsketch.xml
  mimetype.path = $$MSK_PREFIX/share/mime/packages

  desktop.files = molsketch.desktop
  desktop.path = $$MSK_PREFIX/share/applications
  
  appdata.files = net.sourceforge.molsketch.appdata.xml
  appdata.path = $$MSK_PREFIX/share/metainfo
}

INCLUDEPATH += \
        $$MSK_SOURCE_BASE/libmolsketch \
        $$MSK_SOURCE_BASE/obabeliface
