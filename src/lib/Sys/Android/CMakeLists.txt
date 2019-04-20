#find_package(UDEV)

#if(UDEV_FOUND)

include_directories(
${POCO_INCLUDE_DIRS}
${UDEV_INCLUDE_DIRS}
)

add_library(ommsys-android
#STATIC    # doesn't work on all linux versions of the linker
SysImplAndroid.cpp
)

target_link_libraries(ommsys-android
${LIB_POCO_FOUNDATION}
#${UDEV_LIBRARIES}
)

install(TARGETS ommsys-android
LIBRARY DESTINATION lib
ARCHIVE DESTINATION lib
)

#endif(UDEV_FOUND)
