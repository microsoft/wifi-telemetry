find_library(LIBSYSTEMD
    NAMES libsystemd.so
    REQUIRED)

if(LIBSYSTEMD)
    set(LIBSYSTEMD_TARGET ${LIBSYSTEMD})
endif()

MESSAGE(STATUS LIBSYSTEMD=${LIBSYSTEMD})