set(OMMGUI_CONSOLE_SRC
ColorImpl.cpp
ViewImpl.cpp
ModelImpl.cpp
ApplicationImpl.cpp
LabelImpl.cpp
ButtonImpl.cpp
ScrollAreaImpl.cpp
ClusterImpl.cpp
NavigatorImpl.cpp
SliderImpl.cpp
ImageImpl.cpp
WebBrowserImpl.cpp
TextLineImpl.cpp
DragImpl.cpp
SelectionViewImpl.cpp
SplitterImpl.cpp
SelectorImpl.cpp
SwitchImpl.cpp
GraphicImpl.cpp
)


add_library(ommgui-console
${OMMGUI_CONSOLE_SRC}
)


target_link_libraries(ommgui-console
${POCO_FOUNDATION}
${POCO_UTIL}
)


install(TARGETS ommgui-console
RUNTIME DESTINATION bin
LIBRARY DESTINATION lib
ARCHIVE DESTINATION lib
)


set(LIB_OMMGUI_CONSOLE ommgui-console
CACHE INTERNAL lib_ommgui_platform
)
set(LIB_OMMGUI_CONSOLE_DIR Console
CACHE INTERNAL lib_ommgui_platform_dir
)
