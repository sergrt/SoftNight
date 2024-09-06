cmake_minimum_required(VERSION 3.15)

project(SoftNight)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Search prebuilt 3rd party dependencies in those folders
list(APPEND CMAKE_PREFIX_PATH "${THIRDPARTY_DIR}")

find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

include_directories("${THIRDPARTY_DIR}/json/include")

set(SOURCE
    app.cpp
    hotkey.cpp
    hotkey_input.cpp
    main_window.cpp
    settings.cpp
    taskbar_icon.cpp)

set(HEADER
    app.h
    color_correction.h
    color_settings.h
    hotkey.h
    hotkey_input.h
    main_window.h
    settings.h
    taskbar_icon.h
)

set(RESOURCES
    resource.h
    soft_night.rc
    day_and_night.ico
    ${wxWidgets_INCLUDE_DIRS}/wx/msw/wx_dpi_aware.manifest
)
source_group("Resources" FILES ${RESOURCES})
configure_file(day_and_night.ico ${CMAKE_CURRENT_BINARY_DIR}/day_and_night.ico COPYONLY)

add_executable(${PROJECT_NAME} WIN32 ${SOURCE} ${HEADER} ${RESOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES})

target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
