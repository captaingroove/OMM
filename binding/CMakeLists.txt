find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

#set(Python_ADDITIONAL_VERSIONS "3")
#set(Python_ADDITIONAL_VERSIONS "3.4")
#set(Python_ADDITIONAL_VERSIONS "3.4.3")
#find_package(PythonLibs 3 REQUIRED)

#find_package(PythonLibs REQUIRED)
#message(STATUS "Found python include dirs: " ${PYTHON_INCLUDE_DIRS})
#message(STATUS "Found python libs: " ${PYTHON_LIBRARIES})

# python 3 is not found, have to set it manually
set(PYTHON_INC /usr/include/python3.4)
set(PYTHON_LIB /usr/lib/x86_64-linux-gnu/libpython3.4m.so)
set(PYTHON_INCLUDE_DIR ${PYTHON_INC})
set(PYTHON_INCLUDE_DIRS ${PYTHON_INC})
set(PYTHON_INCLUDE_PATH ${PYTHON_INC})
set(PYTHON_LIBRARY ${PYTHON_LIB})
set(PYTHON_LIBRARIES ${PYTHON_LIB})

include_directories(${PYTHON_INCLUDE_PATH})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

message(STATUS "Using python include dir: " ${PYTHON_INCLUDE_DIR})
#message(STATUS "Using python include path: " ${PYTHON_INCLUDE_PATH})
message(STATUS "Using python lib: " ${PYTHON_LIBRARY})

set(PY_MOD_UPNP upnp)
set_source_files_properties(${PY_MOD_UPNP}.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties(${PY_MOD_UPNP}.i PROPERTIES SWIG_FLAGS "-threads")
swig_add_module(${PY_MOD_UPNP} python ${PY_MOD_UPNP}.i)
swig_link_libraries(${PY_MOD_UPNP} ${PYTHON_LIBRARIES} ${OMM} ${OMM_LOG} ${OMM_UTIL} ${OMM_NET} ${OMM_SYS})

set(PY_MOD_UPNP upnpav)
set_source_files_properties(${PY_MOD_UPNP}.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties(${PY_MOD_UPNP}.i PROPERTIES SWIG_FLAGS "-threads")
swig_add_module(${PY_MOD_UPNP} python ${PY_MOD_UPNP}.i)
swig_link_libraries(${PY_MOD_UPNP} ${PYTHON_LIBRARIES} ${OMM} ${OMM_AV} ${OMM_LOG} ${OMM_UTIL} ${OMM_NET} ${OMM_SYS})
