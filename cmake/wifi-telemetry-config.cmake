
include(CMakeFindDependencyMacro)

find_library(LIBSYSTEMD NAMES libsystemd.so)
find_library(LIBWPA_CLIENT NAMES libwpa_client.so)
find_dependency(LTTngUST)
find_dependency(Threads)

include("${CMAKE_CURRENT_LIST_DIR}/wifi-telemetry-targets.cmake")
