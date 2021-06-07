
set(SDBUS_CPP_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/sdbus-cpp-prefix)

ExternalProject_Add(sdbus-cpp
    GIT_REPOSITORY "https://github.com/Kistler-Group/sdbus-cpp.git"
    GIT_TAG "v0.8.3"
)

set(SDBUS_CPP_EXTERNAL_LIBS
    "${SDBUS_CPP_PREFIX}/src/sdbus-cpp-build"
    CACHE PATH
    "Directory of external sdbus-c++ libraries"
)
set(SDBUS_CPP_EXTERNAL_NAME
    sdbus-c++
    CACHE STRING
    "sdbus-c++ shared object name"
)
set(SDBUS_CPP_EXTERNAL
    ${SDBUS_CPP_EXTERNAL_LIBS}/lib${SDBUS_CPP_EXTERNAL_NAME}.so
    CACHE FILEPATH
    "sdbus-c++ shared object"
)
