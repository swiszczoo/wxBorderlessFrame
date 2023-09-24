cmake_minimum_required(VERSION 3.0)
project(wxBorderlessFrame)
include(FindPkgConfig.cmake)

option(WXBF_EXAMPLE "Compile wxBorderlessFrame xample" OFF)
option(WXBF_SHARED "Build shared library" OFF)

set(WXW_USE_DEBUG OFF)
set(WXW_USE_UNICODE OFF)
set(WXW_USE_SHARED ON)
set(WXW_USE_UNIV OFF)
set(WXW_USE_MONO OFF)

set(WXW_FILE_VERSION "32")
set(WXW_VERSION "3.2")

find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})
add_definitions( ${WXWIDGETS_DEFINITIONS} )

set(LIB_SOURCES
    src/borderless_frame_base.cpp
    src/borderless_frame_gtk.cpp
    src/borderless_frame_msw.cpp
    src/drop_shadow_frame_base.cpp
    src/drop_shadow_frame_gtk.cpp
    src/drop_shadow_frame_msw.cpp
    src/system_buttons_base.cpp
    src/system_buttons_fallback.cpp
    src/system_buttons_win10.cpp
    src/system_buttons.cpp
    src/system_menu_png.cpp
    src/window_gripper_gtk.cpp
    src/window_gripper_msw.cpp
    src/window_gripper.cpp
)

set(EXAMPLE_SOURCES example/main.cpp)

if(UNIX AND NOT APPLE)
    add_definitions( -DLINUX=1 )
    PKGCONFIG("gtk+-3.0")
    if(PKGCONFIG_FOUND)
        if(CMAKE_PKGCONFIG_C_FLAGS)
            set(CMAKE_CXX_FLAGS "${CMAKE_PKGCONFIG_C_FLAGS} ${CMAKE_CXX_FLAGS}")
            set(WXWIDGETS_LIBRARIES "${PKGCONFIG_LIBRARIES} ${WXWIDGETS_LIBRARIES}")
            #do something with ${PKGCONFIG_LIBRARIES}
        endif(CMAKE_PKGCONFIG_C_FLAGS)
    else(PKGCONFIG_FOUND)
        message("Cannot find gtk+-3.0")
    endif(PKGCONFIG_FOUND)
endif(UNIX AND NOT APPLE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}${WXWIDGETS_CXX_FLAGS}")

if(WXBF_SHARED)
    set(WXBF_LIBRARY_TYPE SHARED)
else()
    set(WXBF_LIBRARY_TYPE STATIC)
endif()

add_library(${PROJECT_NAME} ${WXBF_LIBRARY_TYPE} ${LIB_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include PRIVATE ${wxWidgets_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${wxWidgets_LINK_DIRECTORIES})
target_compile_definitions(${PROJECT_NAME} PRIVATE BFRAME_DLL_EXPORT)
target_link_libraries(${PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES})
if(WXBF_SHARED)
    target_compile_definitions(${PROJECT_NAME} PUBLIC BFRAME_SHARED_LIBRARY)
    target_compile_definitions(${PROJECT_NAME} INTERFACE BFRAME_SHARED_LIBRARY)
endif()

if(WXBF_EXAMPLE)
    add_executable(example WIN32 ${EXAMPLE_SOURCES})
    target_link_libraries(example PUBLIC ${PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES})
endif()
