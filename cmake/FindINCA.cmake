find_file(
  ETAS_INCACOM_TLB
  NAMES 
    incacom.tlb
  PATHS
    "[HKEY_LOCAL_MACHINE/SOFTWARE/ETAS/INCA/7.4;Path]cebra"
  REGISTRY_VIEW
    BOTH
  DOC 
    "Path to the ETAS INCA 7.4 binary directory."
)
mark_as_advanced(ETAS_INCACOM_TLB)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  INCA
  REQUIRED_VARS 
      ETAS_INCACOM_TLB
)