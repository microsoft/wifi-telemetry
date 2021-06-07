
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/wifi-telemetryd-ap@.service.in
    ${CMAKE_CURRENT_BINARY_DIR}/wifi-telemetryd-ap@.service
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/wifi-telemetryd-station@.service.in
    ${CMAKE_CURRENT_BINARY_DIR}/wifi-telemetryd-station@.service
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/wifi-telemetryd-ztp.service.in
    ${CMAKE_CURRENT_BINARY_DIR}/wifi-telemetryd-ztp.service
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/wifi-telemetryd-ap@.service
        ${CMAKE_CURRENT_BINARY_DIR}/wifi-telemetryd-station@.service
        ${CMAKE_CURRENT_BINARY_DIR}/wifi-telemetryd-ztp.service
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/systemd/system
)
