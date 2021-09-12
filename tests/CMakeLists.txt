find_package( GTest REQUIRED )


if( TANZ_ENABLE_EIGEN )
  set( eigen_files
    check-binary-serialisation-eigen.c++
    check-opengl-transforms.c++
    check-homography.c++
    check-time-measurement.c++
    )
  set( eigen_libs
    eigen )
endif()

add_executable(
  check-all
  check-c++.c++
  check-binary-serialisation.c++
  check-hash-combiner.c++
  check-union-find.c++
  check-object-fifo.c++
  ${eigen_files}
  )

target_link_libraries(
  check-all
  PRIVATE
  tanz
  ${eigen_libs}
  GTest::gtest_main )
