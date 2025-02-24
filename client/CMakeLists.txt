# 设置 CMake 的最低版本要求为 3.16
cmake_minimum_required(VERSION 3.16)

# 定义项目名称为 'client'，版本号为 0.1，使用的编程语言为 C++
project(client VERSION 0.1 LANGUAGES CXX)

# 启用 Qt 的自动化功能：
# - CMAKE_AUTOUIC：自动处理 UI 文件 (.ui)
# - CMAKE_AUTOMOC：自动处理 MOC 文件
# - CMAKE_AUTORCC：自动处理资源文件 (.qrc)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 设置 C++ 标准为 C++17，并要求该标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt 库，要求至少包含 Qt5 或 Qt6，并且必须包含 Widgets 模块
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia)
# 根据找到的 Qt 版本，决定使用 Qt5 还是 Qt6
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia)

# 设置项目源文件列表，当前仅包含 main.cpp 文件
set(PROJECT_SOURCES
        main.cpp
)

# 添加头文件目录，必须要在添加子目录上面
include_directories(
    ${PROJECT_SOURCE_DIR}/card
    ${PROJECT_SOURCE_DIR}/control
    ${PROJECT_SOURCE_DIR}/player
    ${PROJECT_SOURCE_DIR}/strategy
    ${PROJECT_SOURCE_DIR}/thread
    ${PROJECT_SOURCE_DIR}/windows
)


# 添加子目录，通常是为了将其他模块（如 card、windows、player 等）包含进来
# 这些子目录应包含各自的 CMakeLists.txt 文件
add_subdirectory(card)
add_subdirectory(windows)
add_subdirectory(player)
add_subdirectory(thread)
add_subdirectory(strategy)
add_subdirectory(control)

# 添加资源文件
qt5_add_resources(RES res.qrc)


# 如果使用的是 Qt 6 或更高版本，则创建可执行文件
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(client
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    # 在 Qt 6 中，还可以设置 Android 项目的源目录，如下所示：
    # set_property(TARGET client APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #             ${CMAKE_CURRENT_SOURCE_DIR}/android)
else()
    # 如果是 Qt 5 或更低版本，且是 Android 平台时，创建共享库
    if(ANDROID)
        add_library(client SHARED
            ${PROJECT_SOURCES}
        )
        # Android 配置的设置可以通过此处进行调整
        # set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        # 如果不是 Android，创建一个普通的可执行文件,并且把变量RES里面的资源文件一起打包到client执行程序中
        add_executable(client
            ${PROJECT_SOURCES}
            ${RES}
        )
    endif()
endif()

# 链接 Qt 的 Widgets 库以及其他添加的子模块（如 card、windows 等）
target_link_libraries(client PRIVATE Qt${QT_VERSION_MAJOR}::Widgets card windows control player strategy thread)

# 以下是 macOS/iOS 配置部分，用于设置应用程序的标识符等
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.client)
endif()

# 设置目标属性，例如：macOS 平台的捆绑版本号、可执行文件等
set_target_properties(client PROPERTIES
    ${BUNDLE_ID_OPTION} # 在 macOS/iOS 上设置 Bundle ID
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION} # 设置捆绑版本
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} # 设置捆绑的简短版本号
    MACOSX_BUNDLE TRUE # 标记为 macOS 捆绑应用
    WIN32_EXECUTABLE TRUE # 标记为 Windows 可执行文件
)

# 配置安装路径
include(GNUInstallDirs)
install(TARGETS client
    BUNDLE DESTINATION . # 将应用捆绑包安装到当前目录
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} # 库文件安装路径
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # 可执行文件安装路径
)

# 如果使用的是 Qt 6，则调用 qt_finalize_executable 来进行最终的设置
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(client)
endif()
