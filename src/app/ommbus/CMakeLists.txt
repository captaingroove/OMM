if(NOT ANDROID)
add_executable(ommbus
OmmBus.cpp
)

target_link_libraries(ommbus
${POCO_UTIL}
${POCO_NET}
)
install(TARGETS ommbus
RUNTIME DESTINATION bin
)

endif(NOT ANDROID)
