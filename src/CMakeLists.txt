cmake_minimum_required(VERSION 3.16)

project(UwpControl VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.10)

qt_add_executable(appUwpControl
    main.cpp
)

qt_add_qml_module(appUwpControl
    URI UwpControl
    QML_FILES
        Main.qml
        SOURCES uwpremove.h uwpremove.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appUwpControl PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appUwpControl
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appUwpControl
    PRIVATE Qt6::Quick
)

if(WIN32)
    set(RC_FILE "${CMAKE_CURRENT_BINARY_DIR}/manifest.rc")

    set(MANIFEST_CONTENT [=[
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0">
  <trustInfo xmlns="urn:schemas-microsoft-com:asm.v3">
    <security>
      <requestedPrivileges>
        <requestedExecutionLevel level="requireAdministrator" uiAccess="false"/>
      </requestedPrivileges>
    </security>
  </trustInfo>
</assembly>
]=])
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/manifest.xml" "${MANIFEST_CONTENT}")

    file(WRITE "${RC_FILE}" "1 24 \"manifest.xml\"\n")

    target_sources(appUwpControl PRIVATE "${RC_FILE}")
endif()



install(TARGETS appUwpControl
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
