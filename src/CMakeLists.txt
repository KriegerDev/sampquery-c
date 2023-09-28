add_library(sampquery STATIC
    "header.c"
    "packet.c"
    "request.c"
    "debug.c"
)

target_include_directories(sampquery 
    PUBLIC "${LIBRARY_DIR}"
    PRIVATE "${LIBRARY_DIR}/internal"
)

install(TARGETS sampquery DESTINATION "${CMAKE_SOURCE_DIR}/bin")