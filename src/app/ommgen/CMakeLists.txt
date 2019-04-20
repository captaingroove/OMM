add_executable(ommgen
main.cpp
ommgen.cpp
)

if(POCO_JSON)
target_link_libraries(ommgen
${POCO_UTIL}
${POCO_JSON}
${OMM_UTIL}
${OMM}
)
else(POCO_JSON)
target_link_libraries(ommgen
${POCO_UTIL}
${OMM_UTIL}
${OMM}
)
endif(POCO_JSON)

install(TARGETS ommgen
RUNTIME DESTINATION bin
)
