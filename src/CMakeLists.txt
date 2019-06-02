
# The plugin's main shared library
add_library(ldprometheus
    SHARED
    PrometheusPublisherFactory
    PrometheusStatsPublisher
    main
)

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