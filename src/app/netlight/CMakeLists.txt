add_executable(ommlight
DimmableLight.cpp
DimmableLightImpl.cpp
DimmableLightApp.cpp
)

add_executable(ommlightctrl
DimmableLightCtrl.cpp
DimmableLightCtrlImpl.cpp
DimmableLightCtrlAppl.cpp
)

set(OMMLIGHT_LIBS
${POCO_UTIL}
${OMM}
)
if(POCO_JSON)
set(OMMLIGHT_LIBS
${OMMLIGHT_LIBS}
${POCO_JSON}
)
endif(POCO_JSON)

target_link_libraries(ommlight
${OMMLIGHT_LIBS}
)

target_link_libraries(ommlightctrl
${OMMLIGHT_LIBS}
)

install(TARGETS ommlight ommlightctrl
RUNTIME DESTINATION bin
LIBRARY DESTINATION lib
)
