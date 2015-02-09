include_directories(
${OMM_INCLUDE_DIR}/Web
)

set(OMM_WEB_LIB_SRCS
WebLogger.cpp
WebRadio.cpp
)

add_library(ommweb
${OMM_WEB_LIB_SRCS}
)

set(OMM_WEB
ommweb
CACHE INTERNAL libommweb
)

target_link_libraries(ommweb
${POCO_FOUNDATION}
${POCO_XML}
${POCO_NET}
${OMM_LOG}
)

install(TARGETS ommweb
LIBRARY DESTINATION lib
ARCHIVE DESTINATION lib
)
