
add_executable(wifi-telemetryd
    ""
)

set_property(
    TARGET wifi-telemetryd
    PROPERTY POSITION_INDEPENDENT_CODE TRUE
)

install(
    TARGETS wifi-telemetryd 
    DESTINATION ${CMAKE_INSTALL_SBINDIR}
)

target_sources(wifi-telemetryd
    PRIVATE
        main.cxx
)

target_include_directories(wifi-telemetryd
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

target_link_libraries(wifi-telemetryd
    PRIVATE
        wifi-telemetry
    INTERFACE
        ${LIBSYSTEMD_TARGET}
        ${LIBWPA_CLIENT_TARGET}
)

add_subdirectory(systemd)
