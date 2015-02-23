QT += widgets printsupport

DEFINES += QMAKEBUILD

CONFIG += silent

qtVersionSuffix=""
equals(QT_MAJOR_VERSION, 5) {
    qtVersionSuffix= -qt5
}
DEFINES += "QTVERSIONSUFFIX=\"\\\"$$qtVersionSuffix\\\"\""
