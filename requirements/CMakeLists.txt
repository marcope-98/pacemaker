find_package(LATEX REQUIRED COMPONENTS LATEXMK)

if (${LATEX_FOUND})
  add_custom_target(requirements ALL)
  add_custom_command(
    TARGET requirements POST_BUILD
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
  )
  add_custom_command(
    TARGET requirements POST_BUILD
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
  )
endif()