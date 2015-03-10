add_executable(plain WIN32
Plain.cpp
)

target_link_libraries(plain
${OMM_GUI}
)


add_executable(label WIN32
Label.cpp
)

target_link_libraries(label
${OMM_GUI}
)


add_executable(buttonsubclass WIN32
ButtonSubclass.cpp
)

target_link_libraries(buttonsubclass
${OMM_GUI}
)


add_executable(buttoncontroller WIN32
ButtonController.cpp
)

target_link_libraries(buttoncontroller
${OMM_GUI}
)


add_executable(group WIN32
Group.cpp
)

target_link_libraries(group
${OMM_GUI}
)


add_executable(layout WIN32
Layout.cpp
)

target_link_libraries(layout
${OMM_GUI}
)


add_executable(group1tonmodel WIN32
Group1toNModel.cpp
)

target_link_libraries(group1tonmodel
${OMM_GUI}
)


add_executable(stringlist WIN32
StringList.cpp
)

target_link_libraries(stringlist
${OMM_GUI}
)


add_executable(list WIN32
List.cpp
)

target_link_libraries(list
${OMM_GUI}
)


add_executable(itemlist WIN32
ItemList.cpp
)

target_link_libraries(itemlist
${OMM_GUI}
)


add_executable(drag WIN32
Drag.cpp
)

target_link_libraries(drag
${OMM_GUI}
)


add_executable(draglist WIN32
DragList.cpp
)

target_link_libraries(draglist
${OMM_GUI}
)


add_executable(lazylist WIN32
LazyList.cpp
)

target_link_libraries(lazylist
${OMM_GUI}
)


add_executable(lazylistbidir WIN32
LazyListBidir.cpp
)

target_link_libraries(lazylistbidir
${OMM_GUI}
)


add_executable(navigator WIN32
Navigator.cpp
)

target_link_libraries(navigator
${OMM_GUI}
)


add_executable(slider WIN32
Slider.cpp
)

target_link_libraries(slider
${OMM_GUI}
)


add_executable(scrollarea WIN32
ScrollArea.cpp
)

target_link_libraries(scrollarea
${OMM_GUI}
)


add_executable(image WIN32
Image.cpp
)

target_link_libraries(image
${OMM_GUI}
)


add_executable(web WIN32
Web.cpp
)

target_link_libraries(web
${OMM_GUI}
)


add_executable(splitter WIN32
Splitter.cpp
)

target_link_libraries(splitter
${OMM_GUI}
)


add_executable(cluster WIN32
Cluster.cpp
)

target_link_libraries(cluster
${OMM_GUI}
)


add_executable(selector WIN32
Selector.cpp
)

target_link_libraries(selector
${OMM_GUI}
)


add_executable(switch WIN32
Switch.cpp
)

target_link_libraries(switch
${OMM_GUI}
)

#install(TARGETS plain label buttonsubclass buttoncontroller group layout
#    group1tonmodel stringlist list itemlist drag draglist lazylist lazylistbidir
#    navigator slider scrollarea image web splitter cluster selector switch
#RUNTIME DESTINATION bin
#LIBRARY DESTINATION lib
#)
