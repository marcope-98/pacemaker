set(DOXYGEN_OUTPUT_DIRECTORY        ${CMAKE_CURRENT_BINARY_DIR})
set(DOXYGEN_STRIP_FROM_PATH         ${CMAKE_CURRENT_SOURCE_DIR}/../src/inca/include)
set(DOXYGEN_LOOKUP_CACHE_SIZE       3)
set(DOXYGEN_EXTRACT_ALL             YES)
set(DOXYGEN_EXTRACT_STATIC          YES)
set(DOXYGEN_CASE_SENSE_NAMES        NO)
set(DOXYGEN_HIDE_SCOPE_NAMES        YES)
set(DOXYGEN_SHOW_INCLUDE_FILES      NO)
set(DOXYGEN_SORT_MEMBER_DOCS        NO)
set(DOXYGEN_GENERATE_TODOLIST       NO)
set(DOXYGEN_FILE_PATTERNS           "*.hpp")
set(DOXYGEN_RECURSIVE               YES)
set(DOXYGEN_SOURCE_BROWSER          YES)
set(DOXYGEN_REFERENCED_BY_RELATION  YES)
set(DOXYGEN_REFERENCES_RELATION     YES)
set(DOXYGEN_HTML_COLORSTYLE_HUE     205)
set(DOXYGEN_HTML_COLORSTYLE_SAT      86)
set(DOXYGEN_DISABLE_INDEX           YES)
set(DOXYGEN_GENERATE_TREEVIEW       YES)
set(DOXYGEN_FORMULA_FONTSIZE        16)
set(DOXYGEN_MATHJAX_RELPATH         "http://cdn.mathjax.org/mathjax/latest")
set(DOXYGEN_GENERATE_LATEX          NO)
set(DOXYGEN_MACRO_EXPANSION         YES)
set(DOXYGEN_SEARCH_INCLUDES         NO)
set(DOXYGEN_ALLEXTERNALS            YES)
set(DOXYGEN_TEMPLATE_RELATIONS      YES)
set(DOXYGEN_CALL_GRAPH              YES)
set(DOXYGEN_CALLER_GRAPH            YES)
set(DOXYGEN_DOT_GRAPH_MAX_NODES     200)
set(DOXYGEN_MAX_DOT_GRAPH_DEPTH     1)
set(DOXYGEN_DOT_MULTI_TARGETS       YES)
set(DOXYGEN_USE_MDFILE_AS_MAINPAGE  ${CMAKE_CURRENT_SOURCE_DIR}/index.html.md)

find_package(Doxygen REQUIRED dot)

if(${DOXYGEN_FOUND})
  doxygen_add_docs(
    doc 
    # Main page
    ${CMAKE_CURRENT_SOURCE_DIR}/index.html.md
    # Source file documentation
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/inca/include/pacemaker/inca 
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/inca/include/pacemaker/inca/com 
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/inca/include/pacemaker/inca/detail
    ALL
  )
endif()

