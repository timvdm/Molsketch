QT += widgets printsupport
contains(CONFIG, static) { QT += svg }

CONFIG += silent

qtVersionSuffix=""
equals(QT_MAJOR_VERSION, 5) {
    qtVersionSuffix= -qt5
}
DEFINES += QMAKEBUILD \
	"QTVERSIONSUFFIX=\"\\\"$$qtVersionSuffix\\\"\""

sourceDir = $$_PRO_FILE_PWD_
message("Sources in: $$sourceDir")

VERSION = $$cat(version)
VERSIONNICK = $$cat(versionnick)


########### Reading version and default paths
message("Installation directories and corresponding qmake variables:")

defaultVars = \
	Global_prefix,MSK_INSTALL_PREFIX,/usr/local,C:/Program_Files/MolsKetch;\
	Executable,MSK_INSTALL_BINS,/bin,;\
	Headers,MSK_INSTALL_INCLUDES,/include/molsketch,/include;\
	Molecule_library,MSK_INSTALL_LIBRARY,/share/molsketch/library,/library;\
	Custom_molecule_library,MSK_INSTALL_CUSTOM,/share/molsketch/library/custom,/library/custom;\
	Documentation,MSK_INSTALL_DOCS,/share/doc/molsketch,/doc;
contains(QT_ARCH, ".*64.*") : defaultVars += Libraries,MSK_INSTALL_LIBS,/lib64,;
else : defaultVars += Libraries,MSK_INSTALL_LIBS,/lib,;

defaultVars += Version,MSK_VERSION,$$cat(version);\
	Nickname,MSK_Version_NICK,$$cat(versionnick)

for(DV, $$list($$split(defaultVars, ;))) {
	variableDescription = $$section(DV, ",", 0, 0)
	variableName = $$section(DV, ",", 1, 1)
	variableDefaultValue = $$section(DV, ",", 2, 2)
	contains(variableName, ".*INSTALL.*"){
		win*: variableDefaultValue = $${MSK_INSTALL_PREFIX}$$section(DV, ",", 3, 3)
	}
	isEmpty($$variableName) : $$variableName = $$split(variableDefaultValue, _)
	DEFINES += "$${variableName}=\"\\\"$$eval($$variableName)\\\"\""
	message("$${variableDescription}: $${variableName} = $$eval($$variableName)")
}

VERSION = $${MSK_VERSION}
