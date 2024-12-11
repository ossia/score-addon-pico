cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
if(NOT TARGET score_lib_base)
  include("${SCORE_SOURCE_DIR}/cmake/ScoreExternalAddon.cmake")
endif()
include(ScoreAvndHelper)

project(score_addon_pico LANGUAGES CXX)

set(AVND_FOLDER "${3RDPARTY_FOLDER}/avendish")

add_library(score_addon_pico
  Pico/ApplicationPlugin.hpp
  Pico/ApplicationPlugin.cpp
  Pico/DeviceIO.hpp
  Pico/DeviceIO.cpp
  Pico/DeviceGraph.hpp
  Pico/DeviceGraph.cpp
  Pico/ExportDialog.hpp
  Pico/ExportDialog.cpp
  Pico/BasicSourcePrinter.hpp
  Pico/BasicSourcePrinter.cpp
  Pico/ESPSourcePrinter.hpp
  Pico/ESPSourcePrinter.cpp
  Pico/SourcePrinter.hpp
  Pico/SourcePrinter.cpp
  Pico/LibossiaGenerator.hpp
  Pico/LibossiaGenerator.cpp
  Pico/OSCQueryGenerator.hpp
  Pico/OSCQueryGenerator.cpp
  Pico/Pruner.hpp
  Pico/Pruner.cpp

  score_addon_pico.hpp
  score_addon_pico.cpp
)

target_compile_definitions(score_addon_pico PRIVATE "PICO_SOURCE_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\"")
target_link_libraries(score_addon_pico PRIVATE score_plugin_protocols)

avnd_score_plugin_init(BASE_TARGET score_addon_pico)

macro(add_math_process FuncName)
  string(TOLOWER "${FuncName}" SmallFuncName)
  avnd_score_plugin_add(
    BASE_TARGET score_addon_pico
    SOURCES "${AVND_FOLDER}/examples/Advanced/Utilities/Logic.hpp"
    TARGET avnd_blocks_${SmallFuncName}
    MAIN_CLASS ${FuncName}
    NAMESPACE avnd::blocks
  )
endmacro()

add_math_process(Abs)
add_math_process(Sin)
add_math_process(Cos)
add_math_process(Floor)
add_math_process(Trunc)
add_math_process(Round)
add_math_process(Modulo)
add_math_process(Min)
add_math_process(Max)
add_math_process(Lerp)
add_math_process(Clamp)
add_math_process(Equal)
add_math_process(Different)
add_math_process(Greater)
add_math_process(GreaterEqual)
add_math_process(Lesser)
add_math_process(LesserEqual)

add_math_process(Spigot)
add_math_process(Choice)

avnd_score_plugin_add(
  BASE_TARGET score_addon_pico
  SOURCES "${AVND_FOLDER}/examples/Advanced/Utilities/LFO.hpp"
  TARGET avnd_blocks_lfo_x
  MAIN_CLASS LFO
  NAMESPACE ao
)

avnd_score_plugin_add(
  BASE_TARGET score_addon_pico
  SOURCES "${AVND_FOLDER}/examples/Advanced/Utilities/Automation.hpp"
  TARGET automation
  MAIN_CLASS Automation
  NAMESPACE ao
)

avnd_score_plugin_add(
  BASE_TARGET score_addon_pico
  SOURCES "${AVND_FOLDER}/examples/Advanced/Utilities/Mapping.hpp"
  TARGET mapping
  MAIN_CLASS Mapping
  NAMESPACE ao
)
avnd_score_plugin_add(
  BASE_TARGET score_addon_pico
  SOURCES "${AVND_FOLDER}/examples/Advanced/Utilities/Soundfile.hpp"
  TARGET soundfile
  MAIN_CLASS Soundfile
  NAMESPACE ao
)

avnd_score_plugin_finalize(
  BASE_TARGET score_addon_pico
  CUSTOM_PLUGIN
)
