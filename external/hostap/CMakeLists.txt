
find_program(MAKE_EXE 
    NAMES gmake nmake make
    REQUIRED
)

set(HOSTAP_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/hostap-prefix/src/hostap/wpa_supplicant)

ExternalProject_Add(hostap
    GIT_REPOSITORY "http://w1.fi/hostap.git"
    GIT_TAG "5f082c158c49a23614b69e4465e9a7299f004f5f"
    CONFIGURE_COMMAND cp -n ${CMAKE_CURRENT_SOURCE_DIR}/.config ${HOSTAP_PREFIX}/.config
    BINARY_DIR ${HOSTAP_PREFIX}
    BUILD_COMMAND QUIET=1 $(MAKE) libwpa_client.so
    INSTALL_DIR ${HOSTAP_PREFIX}
    INSTALL_COMMAND QUIET=1 $(MAKE) install
)

set(LIBWPA_CLIENT
    ${HOSTAP_PREFIX}/libwpa_client.so
    CACHE FILEPATH
    "wpa client shared object"
)
