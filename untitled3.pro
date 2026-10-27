QT       += core gui widgets

CONFIG += c++17

# 禁用 Qt 6.0.0 之前的废弃 API（根据你截图里 Qt Creator 的默认配置保留）
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

# 源文件列表 (注意：这里不包含 mainwindow.ui)
SOURCES += \
    main.cpp \
    mainwindow.cpp

# 头文件列表
HEADERS += \
    mainwindow.h