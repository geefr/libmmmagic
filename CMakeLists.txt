cmake_minimum_required(VERSION 3.14)

project(libmmmagic)

set( CMAKE_C_VISIBILITY_PRESET default )
set( CMAKE_CXX_VISIBILITY_PRESET default )

add_library( mmmagic SHARED
  mmmagic-dumpsterfire/asyncthreads.h
  mmmagic-dumpsterfire/asyncthreads.c
  mmmagic-dumpsterfire/minimaidhid.h
  mmmagic-dumpsterfire/minimaidhid.c
  mmmagic-dumpsterfire/mm_lowlevel.h
  mmmagic-dumpsterfire/mm_lowlevel.c
  mmmagic-dumpsterfire/mmmagic.h
  mmmagic-dumpsterfire/mmmagic.c
  mmmagic-dumpsterfire/platform.h
  mmmagic-dumpsterfire/update.h
  mmmagic-dumpsterfire/update.c
)
target_compile_definitions( mmmagic PRIVATE "MMMAGIC_API_EXPORT" )
target_compile_definitions( mmmagic PUBLIC "WINDBLOWS" )
target_link_libraries( mmmagic PUBLIC hid.lib setupapi.lib )

add_executable( mmmagic_blink mmmagic-dumpsterfire/main.c )
target_link_libraries( mmmagic_blink mmmagic )
