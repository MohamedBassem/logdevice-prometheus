include(ExternalProject)

ExternalProject_Add(logdevice
    GIT_REPOSITORY https://github.com/facebookincubator/LogDevice
    GIT_TAG master
    PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
    SOURCE_SUBDIR "logdevice"
    SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/external/logdevice"
    CMAKE_ARGS -DBUILD_TESTS=OFF
    INSTALL_COMMAND make install DESTDIR=${PLUGIN_STAGING_DIR}
    )

# Specify include dir
ExternalProject_Get_Property(logdevice SOURCE_DIR)
ExternalProject_Get_Property(logdevice BINARY_DIR)

set(LOGDEVICE_INCLUDE_DIR
  ${CMAKE_CURRENT_BINARY_DIR}/external/logdevice
  ${BINARY_DIR}/staging/usr/local/include
)

set(LOGDEVICE_LIBS
  ${PLUGIN_STAGING_DIR}/usr/local/lib/liblogdevice.so
)

add_custom_command(TARGET logdevice POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${BINARY_DIR}/staging/usr/local/lib/ ${PLUGIN_STAGING_DIR}/usr/local/lib/
)

set(LOGDEVICE_DEPS
  ${PLUGIN_STAGING_DIR}/usr/local/lib/libprotocol.so
  ${PLUGIN_STAGING_DIR}/usr/local/lib/libthriftcpp2.so
  ${PLUGIN_STAGING_DIR}/usr/local/lib/libtransport.so
  ${PLUGIN_STAGING_DIR}/usr/local/lib/libthrift-core.so
  ${PLUGIN_STAGING_DIR}/usr/local/lib/libthriftprotocol.so
  ${PLUGIN_STAGING_DIR}/usr/local/lib/libfolly.so
)


message(STATUS "Logdevice Includes: ${LOGDEVICE_INCLUDE_DIR}")

mark_as_advanced(
    LOGDEVICE_INCLUDE_DIR
)
