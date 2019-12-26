set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#find_package(Qt5 5.3 COMPONENTS QtCore QtGui QtSvg)
#find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core Widgets Qml Quick REQUIRED)

#add_executable(qtomm
##${OMM_RC_FILE}
#QtOmm.cpp
#)

#target_link_libraries(qtomm Qt5::Widgets)



if(QT5_FOUND)
message("Qt5 found")
#if(WINDOWS)
#set(OMM_RC_FILE omm.rc)
#endif(WINDOWS)

add_executable(qtomm
#${OMM_RC_FILE}
OmmApplication.cpp
)

#target_link_libraries(omm
#${OMM}
#${OMM_UTIL}
#${OMM_AV}
#${OMM_GUI}
#${OMM_UPNPGUI}
#${OMMX_ENGINE_VLC}
#${OMMX_ENGINE_PHONON}
#${OMMX_ENGINE_MPMOVIEPLAYER}
#)

#install(TARGETS omm
#RUNTIME DESTINATION bin
#LIBRARY DESTINATION lib
#)

endif(QT5_FOUND)
