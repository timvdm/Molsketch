message("======= Welcome to Molsketch build =======")
include(variables.pri)
############## SETTINGS

message("Build settings:")
buildVars = $$cat(buildvariables)
for(buildVar, $$list($$split(buildVars, \\n))) {
  variableName = $$getVarName(buildVar)
  isEmpty($$variableName) {
    $$variableName = $$getVarDefault(buildVar)
    contains(variableName, ".*INSTALL.*") : $$variableName = $$MSK_PREFIX$$eval($$variableName)
  }
  message("$$getVarDescription(buildVar): $${variableName} = $$eval($$variableName)")
  write_file($$OUT_PWD/buildvars/$$variableName, $$variableName)
}

################# setting up sub projects

TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += \
        libmolsketch \
        molsketch
!android : SUBDIRS += obabeliface
contains(CONFIG, static) { SUBDIRS -= obabeliface }
# TODO make obabeliface optional


################# making installs
INSTALLS += documentation_en \
            documentation_cs \
            documentation_nl \
            includes
documentation_en.files = $$PWD/doc/en/*
documentation_cs.files = $$PWD/doc/cs/*
documentation_nl.files = $$PWD/doc/nl/*
documentation_en.path = $${MSK_INSTALL_DOCS}/en
documentation_cs.path = $${MSK_INSTALL_DOCS}/cs
documentation_nl.path = $${MSK_INSTALL_DOCS}/nl
includes.files = $$PWD/libmolsketch/*.h
includes.path = $${MSK_INSTALL_INCLUDES}

OTHER_FILES += \
    version \
    versionnick \
    CHANGELOG \
    buildvariables
