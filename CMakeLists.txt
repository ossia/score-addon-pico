cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

if(NOT TARGET score_lib_base)
  include("${SCORE_SOURCE_DIR}/cmake/ScoreExternalAddon.cmake")
endif()

project(score_addon_pico LANGUAGES CXX)

set(AVND_FOLDER "${3RDPARTY_FOLDER}/avendish")

add_library(score_addon_pico

  score_addon_pico.hpp
  score_addon_pico.cpp
)

set(AVND_ADDITIONAL_CLASSES)
set(AVND_CUSTOM_FACTORIES)
function(avnd_add_score_plugin)

  cmake_parse_arguments(AVND "" "TARGET;MAIN_CLASS;NAMESPACE;BASE_TARGET" "SOURCES" ${ARGN})

  if(AVND_NAMESPACE)
    set(AVND_QUALIFIED "${AVND_NAMESPACE}::${AVND_MAIN_CLASS}")
  else()
    set(AVND_QUALIFIED "${AVND_MAIN_CLASS}")
  endif()

  list(GET AVND_SOURCES 0 AVND_MAIN_FILE)

#   if(TARGET avnd_source_parser)
#     set(AVND_REFLECTION_HELPERS_PRE "${CMAKE_BINARY_DIR}/${AVND_TARGET}_avnd.refl.pre.hpp")
#     set(AVND_REFLECTION_HELPERS "${CMAKE_BINARY_DIR}/${AVND_TARGET}_avnd.refl.hpp")
#     add_custom_command(
#       OUTPUT "${AVND_REFLECTION_HELPERS}"
#       COMMAND avnd_source_parser "${AVND_QUALIFIED}" "${AVND_MAIN_FILE}" "${AVND_REFLECTION_HELPERS}"
#       DEPENDS avnd_source_parser
#     )
#   endif()

  configure_file(
    "${CMAKE_SOURCE_DIR}/src/plugins/score-plugin-avnd/prototype.cpp.in"
    "${CMAKE_BINARY_DIR}/${AVND_TARGET}_avnd.cpp"
    @ONLY
    NEWLINE_STYLE LF
  )

  target_sources(${AVND_BASE_TARGET} PRIVATE
    ${AVND_SOURCES}
    "${CMAKE_BINARY_DIR}/${AVND_TARGET}_avnd.cpp"
  )

  if(TARGET avnd_source_parser)
    target_sources(${AVND_BASE_TARGET} PRIVATE
      "${AVND_REFLECTION_HELPERS}"
    )
    target_compile_definitions(${AVND_BASE_TARGET} PRIVATE AVND_USE_TUPLET_TUPLE=1)
  endif()

  if(AVND_NAMESPACE)
    set(txt "namespace ${AVND_NAMESPACE} { struct ${AVND_MAIN_CLASS}; } \n")
    set(txtf "::oscr::custom_factories<${AVND_NAMESPACE}::${AVND_MAIN_CLASS}>(fx, ctx, key); \n")
  else()
    set(txt "struct ${AVND_MAIN_CLASS}; \n")
    set(txtf "::oscr::custom_factories<${AVND_MAIN_CLASS}>(fx, ctx, key); \n")
  endif()

  set(AVND_ADDITIONAL_CLASSES "${AVND_ADDITIONAL_CLASSES}\n${txt}\n" PARENT_SCOPE)
  set(AVND_CUSTOM_FACTORIES "${AVND_CUSTOM_FACTORIES}\n${txtf}\n" PARENT_SCOPE)
endfunction()


avnd_add_score_plugin(
  BASE_TARGET score_addon_pico
  SOURCES "${AVND_FOLDER}/examples/Advanced/Utilities/Math.hpp"
  TARGET avnd_blocks_sin
  MAIN_CLASS Sin
  NAMESPACE avnd::blocks
)


 file(CONFIGURE OUTPUT
      "${CMAKE_BINARY_DIR}/include.pico.cpp"
      CONTENT "${AVND_ADDITIONAL_CLASSES}\nvoid all_custom_factories(auto& fx, auto& ctx, auto& key) { ${AVND_CUSTOM_FACTORIES} }\n"
      NEWLINE_STYLE LF)

setup_score_plugin(score_addon_pico)

target_link_libraries(score_addon_pico PUBLIC score_plugin_engine score_plugin_avnd)
