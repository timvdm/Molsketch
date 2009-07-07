# Builds the translation files
# Based on some work of Patrick Noffke
# http://public.kitware.com/pipermail/cmake/2006-July/010055.html

# QT4_WRAP_TS(lupdate_outputs lrelease_outputs prefix lang1 lang2 ... )
macro(QT4_WRAP_TS lupdate_outputs lrelease_outputs prefix)
  foreach(it ${ARGN})
    set(tsfile ${CMAKE_CURRENT_SOURCE_DIR}/${prefix}_${it}.ts)
#      SET(tsfile ${CMAKE_CURRENT_BINARY_DIR}/translations/${prefix}_${it}.ts)
    add_custom_command(OUTPUT ${tsfile} 
      COMMAND ${QT_LUPDATE_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR} -ts ${tsfile}
    )

    set(qmfile ${CMAKE_CURRENT_BINARY_DIR}/${prefix}_${it}.qm)
    add_custom_command(OUTPUT ${qmfile} 
      COMMAND ${QT_LRELEASE_EXECUTABLE} ${tsfile} -qm ${qmfile}
    )

    set(${lupdate_outputs} ${${lupdate_outputs}} ${tsfile})
    set(${lrelease_outputs} ${${lrelease_outputs}} ${qmfile})
  endforeach(it)
endmacro(QT4_WRAP_TS)

# Create the translation files
qt4_wrap_ts(lupdate_outputs lrelease_outputs molsketch nl cs)

# Add target for updating translation files
add_custom_target(lupdate DEPENDS ${lupdate_outputs})

