add_executable(scanwebradio
ScanWebradio.cpp
)

target_link_libraries(scanwebradio
${OMM_WEB}
)

install(TARGETS scanwebradio
RUNTIME DESTINATION bin
LIBRARY DESTINATION lib
)