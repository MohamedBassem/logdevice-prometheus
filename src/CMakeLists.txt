
# The plugin's main shared library
add_library(ldprometheus
    SHARED
    PrometheusPublisherFactory
    PrometheusStatsPublisher
    PrometheusSettings
    main
)

add_dependencies(ldprometheus logdevice prometheus)

target_include_directories(
    ldprometheus
    PUBLIC
        ${LOGDEVICE_INCLUDE_DIR}
        ${PROMETHEUS_INCLUDE_DIR}
)

# Build our shard library with -fPIC
set_target_properties(
  ldprometheus
  PROPERTIES
  POSITION_INDEPENDENT_CODE True
)

# Link the prometheus library statically
target_link_libraries(
    ldprometheus
    "-Wl,--whole-archive"
    ${PROMETHEUS_LIBRARIES}
    "-Wl,--no-whole-archive"
)


# Building the tests
enable_testing()
include(GoogleTest OPTIONAL RESULT_VARIABLE HAVE_CMAKE_GTEST)
add_executable(prometheus_tests tests/PublisherTest.cpp)
add_dependencies(prometheus_tests ldprometheus)
target_include_directories(
  prometheus_tests
  PUBLIC
    ${GTEST_INCLUDE_DIRS})
target_link_libraries(
  prometheus_tests
  ldprometheus
  ${GTEST_LIBRARY}
  ${PROMETHEUS_LIBRARIES}
  ${LOGDEVICE_LIBS}
  Threads::Threads)
add_test(NAME prometheus_tests COMMAND prometheus_tests)
