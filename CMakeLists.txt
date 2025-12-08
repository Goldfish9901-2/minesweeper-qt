cmake_minimum_required(VERSION 3.15)
project(MineSweeper VERSION 1.0 LANGUAGES CXX)

# ---------------- Qt 自动功能 ----------------
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------- Qt6 查找 ----------------
find_package(Qt6 REQUIRED COMPONENTS
        Core Gui Widgets Sql Svg SvgWidgets Qml QuickControls2 LinguistTools
)

qt_standard_project_setup(
        I18N_TRANSLATED_LANGUAGES zh_CN en_US
)

# ---------------- Translation 文件 ----------------
set(TS_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/res/i18n/MineSweeper_zh_CN.ts
        ${CMAKE_CURRENT_SOURCE_DIR}/res/i18n/MineSweeper_en.ts
)
qt_add_translations(MineSweeper
        TS_FILES ${TS_FILES}
        ${CMAKE_CURRENT_BINARY_DIR}/res/i18n
        RESOURCE_PREFIX /i18n
)

# ---------------- 源文件列表 ----------------
set(SOURCES
        src/main.cpp
        src/mainwindow.cpp include/mainwindow.h include/mainwindow.ui
        src/field.cpp include/field.h include/field.ui
        src/grid.cpp include/grid.h
        src/locale.cpp src/render_utils.cpp src/icon_tests.cpp
        src/LanguageChoiceAction.cpp include/LanguageChoiceAction.h
        src/database.cpp include/database.h include/record.h
        src/recordview.cpp include/recordview.h include/record.ui
        src/Localeable.cpp include/Localeable.h
        src/haptic.cpp include/haptic.h
        src/toast.cpp include/toast.h
        ms.qrc
        ${TS_FILES}
)
set(CMAKE_AUTOUIC_SEARCH_PATHS
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ---------------- 可执行文件 ----------------
qt6_add_executable(MineSweeper
        ${SOURCES}
)

target_link_libraries(MineSweeper PRIVATE
        Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Sql Qt6::Svg Qt6::SvgWidgets Qt6::Qml Qt6::QuickControls2
)

target_include_directories(MineSweeper PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# ---------------- Windows DLL 自动部署 ----------------
if(WIN32)
    set(DIST_DIR "${CMAKE_BINARY_DIR}/dist")
    file(MAKE_DIRECTORY ${DIST_DIR})

    find_program(WINDEPLOYQT_EXECUTABLE NAMES windeployqt HINTS $ENV{Qt6_DIR}/../bin)
    if(NOT WINDEPLOYQT_EXECUTABLE)
        message(FATAL_ERROR "windeployqt.exe not found! Set Qt6_DIR environment variable correctly.")
    endif()

    add_custom_command(TARGET MineSweeper POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:MineSweeper>" "${DIST_DIR}/MineSweeper.exe"
            COMMAND "${WINDEPLOYQT_EXECUTABLE}" "--dir" "${DIST_DIR}" "${DIST_DIR}/MineSweeper.exe"
            COMMENT "Deploying MineSweeper and Qt DLLs to dist folder"
    )
endif()

# ---------------- Linux 安装到 ~/.local ----------------
if(UNIX AND NOT APPLE)
    install(TARGETS MineSweeper
            RUNTIME DESTINATION $ENV{HOME}/.local/bin
    )
endif()

# ---------------- Windows / macOS 资源属性 ----------------
if(WIN32)
    set_target_properties(MineSweeper PROPERTIES WIN32_EXECUTABLE TRUE)
elseif(APPLE)
    set_target_properties(MineSweeper PROPERTIES MACOSX_BUNDLE TRUE)
endif()

# ---------------- 打包为 zip ----------------
set(DIST_DIR "${CMAKE_BINARY_DIR}/dist")
add_custom_target(package
        COMMAND ${CMAKE_COMMAND} -E tar "cfv" "${CMAKE_BINARY_DIR}/MineSweeper.zip" --format=zip "${DIST_DIR}"
        DEPENDS MineSweeper
        COMMENT "Packaging dist folder into MineSweeper.zip"
)

# ---------------- Windows 资源文件 ----------------
if(WIN32)
    target_sources(MineSweeper PRIVATE res/appicon.rc)
endif()

# ---------------- MSVC 编码 ----------------
if(MSVC)
    add_compile_options("/utf-8")
endif()
