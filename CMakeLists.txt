# project options
cmake_minimum_required(VERSION 3.14)
project(wifi-telemetry CXX)

# c++ standard options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# allow setting target_link_libraries() from any dir
cmake_policy(SET CMP0079 NEW)

# define option for use of Microsoft tracelogging
option(
    ENABLE_MS_LTTNG_EXTENSIONS
    "Enable use of Microsoft tracelogging extensions for LTTNG"
    OFF
)

# define option to build hostap dev dependency in-tree
option(
    BUILD_HOSTAP_EXTERNAL
    "Build wpa_supplicant/hostapd from built-in external source"
    ON
)

# define option to build sdbus-c++ dev dependency in-tree
option(
    BUILD_SDBUS_CPP_EXTERNAL
    "Build sdbus-c++ from built-in external source"
    ON
)

include(ExternalProject)
include(GNUInstallDirs)
include(CheckPIESupported)
include(cmake/find_wpa_client.cmake)
include(cmake/find_systemd.cmake)
include(cmake/FindLTTngUST.cmake)
include(cmake/find_pthreads.cmake)
include(cmake/find_sdbus-cpp.cmake)
if (ENABLE_MS_LTTNG_EXTENSIONS)
    include(cmake/find_tracelogging.cmake)
endif()

check_pie_supported()
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# compilation options
add_compile_options(
    -Wall 
    -Wshadow
    -Wformat-security
    -Werror
    -Wextra
    -Wpedantic
    -Wconversion
)

if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
    add_compile_options(
        -fstack-protector
        -fvisibility=hidden
    )
elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(
        -fstack-clash-protection
        -fstack-protector-all
        -fvisibility=hidden
        -Wl,-z,noexecstack
        -Wl,-z,now
        -Wl,-z,relro
    )
endif()

if (CMAKE_BUILD_TYPE MATCHES "(Release|RelWithDebInfo|MinSizeRel)")
    add_compile_definitions(_FORTIFY_SOURCE=2)
endif()

if (ENABLE_MS_LTTNG_EXTENSIONS)
    add_compile_definitions(TRACE_USE_TRACELOGGING)
endif()

include_directories(include)

add_subdirectory(src)
