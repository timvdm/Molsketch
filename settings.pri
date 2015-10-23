QT += widgets printsupport

DEFINES += QMAKEBUILD

CONFIG += silent

qtVersionSuffix=""
equals(QT_MAJOR_VERSION, 5) {
    qtVersionSuffix= -qt5
}
DEFINES += "QTVERSIONSUFFIX=\"\\\"$$qtVersionSuffix\\\"\""

isEmpty(PREFIX) {
	unix : PREFIX = /usr/local
	win32 : PREFIX = "C:/Program Files/MolsKetch"
}

sourceDir = $$_PRO_FILE_PWD_

contains(CONFIG, static) { QT += svg }
