
target_sources(wifi-telemetry
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/wifi_telemetry_lttng_provider_station.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/wifi_telemetry_lttng_provider_dpp.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/wifi_telemetry_lttng_provider_wifi.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/wifi_telemetry_trace_dpp.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/wifi_telemetry_trace_station.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/wifi_telemetry_trace_wifi.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/wifi_telemetry_trace.cxx
)

target_include_directories(wifi-telemetry
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(wifi-telemetry
    INTERFACE
        LTTng::UST
)

if (ENABLE_MS_LTTNG_EXTENSIONS)
    target_link_libraries(wifi-telemetry
        PRIVATE
            tracelogging::tracelogging
    )
endif()
