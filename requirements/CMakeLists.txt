find_package(LATEX REQUIRED COMPONENTS LATEXMK)

if (${LATEX_FOUND})
  add_custom_target(
    requirements ALL
    DEPENDS
      ${CMAKE_CURRENT_BINARY_DIR}/pacemaker_SRS.pdf
      ${CMAKE_CURRENT_BINARY_DIR}/pacemaker_TS.pdf
  )
  file(GLOB SRS_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/chapters/REQ/*.tex)
  add_custom_command(
    OUTPUT  
      ${CMAKE_CURRENT_BINARY_DIR}/pacemaker_SRS.pdf
    COMMAND
      ${LATEXMK_COMPILER} 
      ARGS
        -cd
        -synctex=1 
        -file-line-error 
        -pdf 
        -interaction=nonstopmode 
        -outdir=${CMAKE_CURRENT_BINARY_DIR} 
        ${CMAKE_CURRENT_SOURCE_DIR}/pacemaker_SRS.tex
    DEPENDS 
      ${CMAKE_CURRENT_SOURCE_DIR}/pacemaker_SRS.tex
      ${SRS_SOURCE_FILES}
  )

  file(GLOB TC_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/chapters/TC/*.tex)
  add_custom_command(
    OUTPUT 
      ${CMAKE_CURRENT_BINARY_DIR}/pacemaker_TS.pdf
    COMMAND
      ${LATEXMK_COMPILER}
      ARGS 
        -cd
        -synctex=1 
        -file-line-error 
        -pdf 
        -interaction=nonstopmode 
        -outdir=${CMAKE_CURRENT_BINARY_DIR} 
        ${CMAKE_CURRENT_SOURCE_DIR}/pacemaker_TS.tex
    DEPENDS 
      ${CMAKE_CURRENT_SOURCE_DIR}/pacemaker_TS.tex
      ${TC_SOURCE_FILES}
  )
endif()