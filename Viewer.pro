QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++2a

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    data_read_worker.cpp \
    file_info_widget.cpp \
    file_reader.cpp \
    main.cpp \
    main_window.cpp \
    plot_drawer.cpp \
    plot_widget.cpp \
    preview_plot_frame_item.cpp \
    preview_plot_scene.cpp \
    preview_plot_view.cpp \
    ruler_widget.cpp

HEADERS += \
    data_read_worker.hpp \
    data_stats.hpp \
    file_info_widget.hpp \
    file_reader.hpp \
    header.hpp \
    main_window.hpp \
    measurement.hpp \
    plot_drawer.hpp \
    plot_widget.hpp \
    preview_plot_frame_item.hpp \
    preview_plot_scene.hpp \
    preview_plot_view.hpp \
    ruler_widget.hpp \
    tick_parameters.hpp

FORMS += \
    file_info_widget.ui \
    main_window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
