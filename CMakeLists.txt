cmake_minimum_required(VERSION 3.14)

project(libmmmagic)

# Options
option( MMMAGIC_VERBOSE_LOGGING "Turn on loads of debug text" OFF )
option( MMMAGIC_BUILD_BLINK_EXE "Build mmmagic_blink.exe" ON )

set( CMAKE_C_VISIBILITY_PRESET default )
set( CMAKE_CXX_VISIBILITY_PRESET default )

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set( SIXTY_FOUR_BIT TRUE )
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set( SIXTY_FOUR_BIT FALSE )
endif()

set( MMMAGIC_SRC
  mmmagic/asyncthreads.h
  mmmagic/asyncthreads.c
  mmmagic/minimaidhid.h
  mmmagic/minimaidhid.c
  mmmagic/mm_lowlevel.h
  mmmagic/mm_lowlevel.c
  mmmagic/mmmagic.h
  mmmagic/mmmagic.c
  mmmagic/platform.h
  mmmagic/update.h
  mmmagic/update.c)

# mmmagic.dll
add_library( mmmagic SHARED ${MMMAGIC_SRC} )
target_compile_definitions( mmmagic PRIVATE "MMMAGIC_API_EXPORT" )
target_compile_definitions( mmmagic PUBLIC "WINDBLOWS" )
if( SIXTY_FOUR_BIT )
  set_target_properties(mmmagic PROPERTIES OUTPUT_NAME "mmmagic64")
endif()

add_library( mmmagic_nonthreaded SHARED ${MMMAGIC_SRC} )
target_compile_definitions( mmmagic_nonthreaded PRIVATE "MMMAGIC_API_EXPORT" )
target_compile_definitions( mmmagic_nonthreaded PRIVATE "MMMAGIC_NONTHREADED" )
target_compile_definitions( mmmagic_nonthreaded PUBLIC "WINDBLOWS" )
if( SIXTY_FOUR_BIT )
  set_target_properties(mmmagic_nonthreaded PROPERTIES OUTPUT_NAME "mmmagic64_nonthreaded")
endif()

if(MMMAGIC_VERBOSE_LOGGING)
  target_compile_definitions( mmmagic PUBLIC "DEBUG_VERSION_HID")
  target_compile_definitions( mmmagic_nonthreaded PUBLIC "DEBUG_VERSION_HID")
endif()

target_link_libraries( mmmagic PUBLIC hid.lib setupapi.lib )
target_link_libraries( mmmagic_nonthreaded PUBLIC hid.lib setupapi.lib )

# Test executable
if( MMMAGIC_BUILD_BLINK_EXE )
  add_executable( mmmagic_blink mmmagic/main.cpp )
  target_link_libraries( mmmagic_blink mmmagic )

  add_executable( mmmagic_blink_nonthreaded mmmagic/main.cpp )
  target_link_libraries( mmmagic_blink_nonthreaded mmmagic_nonthreaded )
endif()

# Installation
install(TARGETS mmmagic mmmagic_nonthreaded DESTINATION .)
install(FILES 
  mmmagic/mmmagic.h 
  mmmagic/platform.h 
  DESTINATION .)
