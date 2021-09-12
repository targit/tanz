project( tanz CXX )
cmake_minimum_required( VERSION 3.20 )

option(
  TANZ_BUILD_TESTS
  "Build the tests."
  FALSE )

option(
  TANZ_ENABLE_EIGEN
  "Build parts depending on Eigen3"
  FALSE
  )

set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_POSITION_INDEPENDENT_CODE ON )

set(
  INSTALL_TARGETS_DEFAULT_ARGS
  EXPORT tanzTargets
  ARCHIVE DESTINATION  "lib/"
  LIBRARY DESTINATION  "lib/"
  RUNTIME DESTINATION  "bin/"
  INCLUDES DESTINATION "include/")

set( TANZ_SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR} )

add_subdirectory( tanz )

if( TANZ_BUILD_TESTS )
  add_subdirectory( tests )
endif()

include( CMakePackageConfigHelpers )

install(
  EXPORT tanzTargets
  NAMESPACE tanz::
  DESTINATION lib/cmake/tanz/ )
configure_file( tanzConfig.cmake.in tanzConfig.cmake @ONLY )
install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/tanzConfig.cmake"
  DESTINATION lib/cmake/tanz )

configure_file( tanz.pc.in tanz.pc @ONLY )
install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/tanz-eigen.pc"
  DESTINATION lib/pkgconfig )

if( TANZ_ENABLE_EIGEN )
  configure_file( tanz-eigen.pc.in tanz-eigen.pc @ONLY )
  install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/tanz-eigen.pc"
    DESTINATION lib/pkgconfig/ )
endif()
