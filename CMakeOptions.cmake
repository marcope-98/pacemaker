option(
  PACEMAKER_BUILD_REQUIREMENTS     
  "Enable generation of SRS and TS documents from LaTeX source files. Default: ${PROJECT_IS_TOP_LEVEL}. Values: { ON, OFF }." 
  ${PROJECT_IS_TOP_LEVEL}
)

option(
  PACEMAKER_BUILD_DOCS
  "Enable generation of Doxygen Source Documentation. Default: ${PROJECT_IS_TOP_LEVEL}. Values { ON, OFF }."
  ${PROJECT_IS_TOP_LEVEL}
)

option(
  PACEMAKER_BUILD_EXAMPLES
  "Enable building examples. Default: ${PROJECT_IS_TOP_LEVEL}. Values { ON, OFF }."
  ${PROJECT_IS_TOP_LEVEL}
)

option(
  PACEMAKER_BUILD_TESTS
  "Enable building tests and test infrastructure. Default: ${PROJECT_IS_TOP_LEVEL}. Values: { ON, OFF }."
  ${PROJECT_IS_TOP_LEVEL}
)