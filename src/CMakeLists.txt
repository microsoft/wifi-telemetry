
install(
    EXPORT wifi-telemetry-targets
    NAMESPACE wifi-telemetry::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wifi-telemetry
)

install(
    FILES "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/wifi-telemetry-config.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wifi-telemetry
)

add_subdirectory(daemon)
add_subdirectory(lib)
