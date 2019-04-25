if(NOT ANDROID)
add_executable(ommbus
OmmBus.cpp
)

target_link_libraries(ommbus
${POCO_UTIL}
${OMM}
)
install(TARGETS ommbus
RUNTIME DESTINATION bin
)

endif(NOT ANDROID)
