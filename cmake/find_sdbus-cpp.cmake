
if (BUILD_SDBUS_CPP_EXTERNAL)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../external/sdbus-c++)
else()
    find_package(sdbus-c++ REQUIRED)
endif()