find_path(GLFW_INCLUDE_DIR
GLFW/glfw3.h
PATHS ${CMAKE_INCLUDE_PATH}
)

find_library(GLFW_LIBRARY
NAME glfw
PATHS ${CMAKE_LIBRARY_PATH}
)

set(GLFW_LIBRARIES
${GLFW_LIBRARY}
)

set(GLFW_INCLUDE_DIRS
${GLFW_INCLUDE_DIR}
)

if(GLFW_INCLUDE_DIR)
message(STATUS "Found GLFW headers in: " ${GLFW_INCLUDE_DIR})
else(GLFW_INCLUDE_DIR)
message(STATUS "GLFW headers not found")
endif(GLFW_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLFW DEFAULT_MSG GLFW_LIBRARY GLFW_INCLUDE_DIR)
mark_as_advanced(GLFW_INCLUDE_DIR GLFW_LIBRARY)
