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
equals(MSK_OBABELIFACE, true) : SUBDIRS += obabeliface

################# making installs
INSTALLS += documentation_base \
            documentation_en \
            documentation_cs \
            documentation_nl \
            includes \
            includes_actions
documentation_base.files = $$PWD/doc/*
documentation_en.files = $$PWD/doc/en/*
documentation_cs.files = $$PWD/doc/cs/*
documentation_nl.files = $$PWD/doc/nl/*
documentation_base.path = $${MSK_INSTALL_DOCS}
documentation_en.path = $${MSK_INSTALL_DOCS}/en
documentation_cs.path = $${MSK_INSTALL_DOCS}/cs
documentation_nl.path = $${MSK_INSTALL_DOCS}/nl
includes.files = $$PWD/libmolsketch/*.h
includes_actions.files = $$PWD/libmolsketch/actions/*.h
includes.path = $${MSK_INSTALL_INCLUDES}/libmolsketch
includes_actions.path = $${MSK_INSTALL_INCLUDES}/libmolsketch/actions

OTHER_FILES += \
    version \
    versionnick \
    CHANGELOG \
    buildvariables \
    COPYING
