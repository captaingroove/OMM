include_directories(
../lib/Upnp
../include/Omm
)

#set( OMMTEST_DEVICE_SRCS
#device.cpp
#)

# set( OMMTEST_NETWORK_LIGHT_SRCS
# network_light.cpp
# network_light_implementation.cpp
# network_light_application.cpp
# )

#set(OMMTEST_OMMLIGHT_CTRL_SRCS
#netlight_ctrl.cpp
#netlight_ctrl_impl.cpp
#netlight_ctrl_appl.cpp
#)

#set(OMMTEST_MEDIAOBJECT_SRCS
#MediaObjectTest.cpp
#)

# set( OMMTEST_HTTPFILESERVER_SRCS
# HttpFileServerTest.cpp
# )

 add_executable(ssdplistener
 Ssdp.cpp
 )

# add_executable(device_test
# ${OMMTEST_DEVICE_SRCS}
# )

# add_executable(network_light
# ${OMMTEST_NETWORK_LIGHT_SRCS}
# )

# add_executable(netlight_ctrl
# ${OMMTEST_OMMLIGHT_CTRL_SRCS}
# )
#
# add_executable(media_object_test
# ${OMMTEST_MEDIAOBJECT_SRCS}
# )

# add_executable(file_server_test
# ${OMMTEST_HTTPFILESERVER_SRCS}
# )


 target_link_libraries(ssdplistener
 ${OMM}
 ${POCO_UTIL}
 )

# target_link_libraries(device_test
# ${OMM}
# ${POCO_UTIL}
# )

# target_link_libraries(network_light
# ${OMM}
# ${POCO_UTIL}
# )

# target_link_libraries(netlight_ctrl
# ${OMM}
# ${POCO_UTIL}
# )
#
#
# target_link_libraries(media_object_test
# ${OMM_AV}
# )

# target_link_libraries(file_server_test
# ${OMM_AV}
# ${POCO_UTIL}
# )


add_executable(avstreamstress
../plugin/Engine/AvStream/AvStreamEngine.cpp
AvStreamStress.cpp
)

target_link_libraries(avstreamstress
${POCO_UTIL}
${OMM_UTIL}
${OMM}
${OMM_AV}
${OMM_AVSTREAM}
)


add_executable(bytequeuestress
ByteQueueStress.cpp
)

target_link_libraries(bytequeuestress
${POCO_UTIL}
${OMM_UTIL}
${OMM}
${OMM_AV}
${OMM_AVSTREAM}
)


add_executable(tagtest
TagTest.cpp
)

target_link_libraries(tagtest
${POCO_UTIL}
${OMM_UTIL}
${OMM}
${OMM_AV}
${OMM_AVSTREAM}
)


add_executable(upnpavstress
UpnpAvStress.cpp
)

target_link_libraries(upnpavstress
${POCO_UTIL}
${OMM_UTIL}
${OMM}
${OMM_AV}
)


add_executable(search
SearchCriteria.cpp
)

target_link_libraries(search
${OMM}
${OMM_AV}
)


add_executable(controller
Controller.cpp
)

target_link_libraries(controller
${OMM}
${OMM_AV}
${POCO_UTIL}
)


target_link_libraries(search
${OMM}
${OMM_AV}
)


install(TARGETS ssdplistener upnpavstress tagtest
RUNTIME DESTINATION bin
LIBRARY DESTINATION lib
)