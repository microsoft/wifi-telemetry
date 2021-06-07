
target_include_directories(wifi-telemetry
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

if (BUILD_SDBUS_CPP_EXTERNAL)
    add_dependencies(wifi-telemetry sdbus-cpp)
    add_library(sdbus-cpp-external SHARED IMPORTED)
    set_target_properties(sdbus-cpp-external PROPERTIES
        IMPORTED_LOCATION ${SDBUS_CPP_EXTERNAL}
    )

    target_link_libraries(wifi-telemetry
        PRIVATE
            sdbus-cpp-external
    )
else()
    target_link_libraries(wifi-telemetry
        PRIVATE
            SDBusCpp::sdbus-c++
    )
endif()
