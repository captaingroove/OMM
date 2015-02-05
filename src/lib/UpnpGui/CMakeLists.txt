find_package(LinuxDVB) # needed for DVB specific setup dialogs

include(AddResources)

add_resources(MediaImages
"img/media-server.png"
"img/media-container.png"
"img/media-item.png"
"img/media-audio-item.png"
"img/media-image-item.png"
"img/media-video-item.png"
"img/media-audio-broadcast.png"
"img/media-video-broadcast.png"
"img/media-skip-backward.png"
"img/media-start.png"
"img/media-stop.png"
"img/media-skip-forward.png"
"img/activity-off.png"
"img/activity-on.png"
)

set(LIB_OMM_UPNPAPP_SRC
UpnpApplication.cpp
WebSetup.cpp
)

add_library(ommupnpapp
${LIB_OMM_UPNPAPP_SRC}
)

set(LIB_OMM_UPNPGUI_SRC
ActivityIndicator.cpp
ControllerWidget.cpp
DeviceGroup.cpp
GuiVisual.cpp
MediaRenderer.cpp
MediaServer.cpp
MediaObject.cpp
Playlist.cpp
GuiSetup.cpp
GuiUpnpApplication.cpp
${MediaImages_CPP}
)

add_library(ommupnpgui
${LIB_OMM_UPNPGUI_SRC}
)

add_library(ommupnpguiconsole
${LIB_OMM_UPNPGUI_SRC}
)

set(DEFAULT_ENGINE
)
set(EXTRA_LIBS
${OMM_DVB}
)

if(IPHONE)
set(DEFAULT_ENGINE
ommx-engine-mpmovieplayer
ommx-engine-avfoundation
ommx-model-mpmedia
ommx-model-iphonecamera
)
set(EXTRA_LIBS
)
elseif(IPHONE)
set(DEFAULT_ENGINE
ommx-engine-vlc
)
endif(IPHONE)

set(OMMUPNPAPP_LINK_LIBS
ommsys
ommutil
ommupnp
ommupnpav
${DEFAULT_ENGINE}
${EXTRA_LIBS}
)

if(LINUXDVB_FOUND)
set(OMMUPNPAPP_LINK_LIBS
${OMMUPNPAPP_LINK_LIBS}
ommdvb
)
endif(LINUXDVB_FOUND)

target_link_libraries(ommupnpapp
${OMMUPNPAPP_LINK_LIBS}
)

target_link_libraries(ommupnpgui
ommupnpapp
ommgui
)

target_link_libraries(ommupnpguiconsole
ommupnpapp
ommguiconsole
)

set(OMM_UPNPAPP
ommupnpapp
CACHE INTERNAL ommupnpapp
)

set(OMM_UPNPGUI
ommupnpgui
CACHE INTERNAL ommupnpgui
)

set(OMM_UPNPGUI_CONSOLE
ommupnpguiconsole
CACHE INTERNAL ommupnpguiconsole
)

install(TARGETS ommupnpapp ommupnpgui ommupnpguiconsole
RUNTIME DESTINATION bin
LIBRARY DESTINATION lib
ARCHIVE DESTINATION lib
)
