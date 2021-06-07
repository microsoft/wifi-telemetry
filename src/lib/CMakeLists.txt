
add_library(wifi-telemetry
    SHARED
    ""
)

if (BUILD_HOSTAP_EXTERNAL)
    add_dependencies(wifi-telemetry hostap)
endif()

target_include_directories(wifi-telemetry
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

target_link_libraries(wifi-telemetry
    INTERFACE
        ${LIBSYSTEMD_TARGET}
        ${LIBWPA_CLIENT_TARGET}
        udev
        pci
)

target_sources(wifi-telemetry
    PRIVATE
        wifi_device_info.cxx
        wifi_telemetry_monitor.cxx
        wifi_telemetry_source.cxx
        wifi_telemetry_instance_impl.cxx
        wifi_telemetry_source_wpa.cxx
        wifi_telemetry_source_ztpd.cxx
        ../../include/wifi-telemetry/wifi/wifi_80211_format_mac.hpp
)

add_subdirectory(wpa)
add_subdirectory(lttng-providers)
add_subdirectory(ztpd)

install(
    TARGETS wifi-telemetry
    EXPORT wifi-telemetry-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../include/ 
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} 
)
