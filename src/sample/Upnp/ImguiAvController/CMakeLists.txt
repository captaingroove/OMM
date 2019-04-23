find_package(GLFW)
find_package(OpenGL)

if(GLFW_FOUND AND OpenGL_FOUND)

set(IMGUI_SOURCES
imgui/imgui_impl_glfw.cpp
imgui/imgui_impl_opengl3.cpp
imgui/imgui.cpp
imgui/imgui_demo.cpp
imgui/imgui_draw.cpp
imgui/imgui_widgets.cpp
imgui/GL/gl3w.c
)

include_directories(
${OPENGL_INCLUDE_DIR}
${GLFW_INCLUDE_DIR}
imgui
)

set(OMMC_IMGUIAPP_SRCS
${IMGUI_SOURCES}
main.cpp
)

add_executable(imguiavcontroller
${OMMC_IMGUIAPP_SRCS}
#${OMMC_RES_SRC}
)

target_link_libraries(imguiavcontroller
${OPENGL_LIBRARIES}
${GLFW_LIBRARIES}
${OMM}
${OMM_AV}
${OMM_SYS}
${OMM_UTIL}
dl
)

install(TARGETS imguiavcontroller
RUNTIME DESTINATION bin
LIBRARY DESTINATION lib/omm
ARCHIVE DESTINATION lib
)

endif(GLFW_FOUND AND OpenGL_FOUND)
