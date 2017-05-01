message("======= Welcome to Molsketch build =======")

############## SETTINGS

message("Build settings:")
buildVars = $$cat(buildvariables)
for(buildVar, $$list($$split(buildVars, \\n))) {
  description = $$section(buildVar, ;, 0, 0)
  variableName = $$section(buildVar, ;, 1, 1)
  defaultValue = $$section(buildVar, ;, 2, 2)
  defaultValueWindows = $$section(buildVar, ;, 3, 3)
  isEmpty($$variableName) {
    $$variableName = $$defaultValue
    win*: $$variableName = $$defaultValueWindows
    contains(variableName, ".*INSTALL.*") : $$variableName = $$MSK_PREFIX$$eval($$variableName)
  }
  message("$$description: $${variableName} = $$eval($$variableName)")
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
    buildvariables
