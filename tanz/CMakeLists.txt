add_library(
  tanz
  STATIC
  binary-serialisation.c++
  union-find.c++
  time-measurement.c++
  )

add_library( tanz::tanz ALIAS tanz )

set_target_properties(
  tanz
  PROPERTIES
  OUTPUT_NAME tanz-core )

target_include_directories(
  tanz
  PUBLIC
  $<BUILD_INTERFACE:${TANZ_SRC_ROOT}>
  $<INSTALL_INTERFACE:include/>)

install(
  TARGETS tanz
  ${INSTALL_TARGETS_DEFAULT_ARGS} )

install(
  FILES
  binary-serialisation.h++
  complex.h++
  hash-combiner.h++
  object-fifo.h++
  optional-queued-promise.h++
  propagation-nodes.h++
  sexpr-dumper.h++
  time-measurement.h++
  union-find.h++

  DESTINATION
  include/tanz/
  )


if( TANZ_ENABLE_EIGEN )
  find_package( Eigen3 REQUIRED )

  add_library(
    eigen
    STATIC
    binary-serialisation-eigen.c++
    homography.c++
    conics.c++
    opengl-transforms.c++)

  set_target_properties(
    eigen
    PROPERTIES
    OUTPUT_NAME tanz-eigen )

  add_library( tanz::eigen ALIAS eigen )

  target_link_libraries(
    eigen
    tanz
    Eigen3::Eigen
    )
  target_include_directories(
    eigen
    PUBLIC
    $<BUILD_INTERFACE:${TANZ_SRC_ROOT}>
    $<INSTALL_INTERFACE:include/>
    )

  install(
    TARGETS eigen
    ${INSTALL_TARGETS_DEFAULT_ARGS} )

  install(
    FILES
    binary-serialisation-eigen.h++
    conics.h++
    dsp.h++
    homography.h++
    opengl-transforms.h++
    DESTINATION
    include/tanz/ )
endif()
