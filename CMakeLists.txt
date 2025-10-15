cmake_minimum_required(VERSION 3.15)

project(MineSweeper VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools Svg SvgWidgets)


set(QT_VERSION_MAJOR 6)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
qt_standard_project_setup(
        I18N_TRANSLATED_LANGUAGES zh_CN en_US
)

set(TS_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/res/i18n/MineSweeper_zh_CN.ts
        ${CMAKE_CURRENT_SOURCE_DIR}/res/i18n/MineSweeper_en.ts
)
set(CMAKE_AUTOUIC_SEARCH_PATHS
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)


set(PROJECT_SOURCES
        src/main.cpp

        src/mainwindow.cpp
        include/mainwindow.h
        include/mainwindow.ui

        include/field.ui
        include/field.h
        src/field.cpp

        include/grid.h
        src/grid.cpp

        include/static.h
        include/base_interface.h
        ms.qrc

        ${TS_FILES}
        src/locale.cpp
        src/render_utils.cpp
        src/icon_tests.cpp

        include/LanguageChoiceAction.h
        src/LanguageChoiceAction.cpp
)
if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MineSweeper
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
    )
    # Define target properties for Android with Qt 6 as:
    #    set_property(TARGET MineSweeper APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

#    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
    qt_add_translations(MineSweeper
            TS_FILES ${TS_FILES}
            ${CMAKE_CURRENT_BINARY_DIR}/res/i18n
            RESOURCE_PREFIX /i18n
    )

    #    # 添加生成的.qm文件到资源文件
    set_source_files_properties(ms.qrc PROPERTIES QT_RESOURCE_ALIAS_PATH "/i18n")
    set_source_files_properties(${qmFiles} PROPERTIES
            QT_RESOURCE_ALIAS_PATH "/i18n")

else ()
    if (ANDROID)
        add_library(MineSweeper SHARED
                ${PROJECT_SOURCES}
                ${QM_FILES}
        )
        # Define properties for Android with Qt 5 after find_package() calls as:
        #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else ()
        add_executable(MineSweeper
                ${PROJECT_SOURCES}
                ${QM_FILES}
        )
    endif ()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif ()
if (WIN32)
    target_sources(MineSweeper PRIVATE res/appicon.rc)
endif ()
if (MSVC)
    add_compile_options("/utf-8")
endif ()

set_target_properties(MineSweeper PROPERTIES
        WIN32_EXECUTABLE TRUE
        MACOSX_BUNDLE TRUE
)

target_include_directories(MineSweeper PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(MineSweeper
        PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui

        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Svg
        Qt${QT_VERSION_MAJOR}::SvgWidgets
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if (${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.MineSweeper)
endif ()
set_target_properties(MineSweeper PROPERTIES
        ${BUNDLE_ID_OPTION}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS MineSweeper
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MineSweeper)
endif ()