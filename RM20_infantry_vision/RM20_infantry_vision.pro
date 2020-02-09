TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    algorithm/src/kalman_filter.cpp \
    armor_detect/armor_detect.cpp \
    get_frame/camera/src/hikcamera.cpp \
    get_frame/debug/debug_settings.cpp \
    get_frame/frame/frame.cpp \
    interface/src/CRC_Check.cpp \
    interface/src/interface.cpp \
    interface/src/serial.cpp \
    main/main.cpp \
    watchdog/WatchDog.cpp \
    algorithm/src/usr_math.cpp \
    fanwheel/fan_middleware.cpp \
    fanwheel/fan.cpp \
    fanwheel/get_binary.cpp \
    fanwheel/find_armor.cpp \
    fanwheel/find_flow_strip_fan.cpp \
    fanwheel/find_flow_strip.cpp \
    fanwheel/find_center.cpp \
    fanwheel/get_predict.cpp \
    fanwheel/fan_param.cpp \
    main/thread.cpp \
    fanwheel/detect_mode.cpp


LIBS += /usr/local/lib/libopencv_*\
        /usr/local/lib/hik_vision/lib*
LIBS += -L/lib/x86_64-linux-gnu/ -lpthread

HEADERS += \
    algorithm/include/kalman_filter.hpp \
    armor_detect/armor_detect.h \
    get_frame/camera/include/CameraParams.h \
    get_frame/camera/include/hikcamera.h \
    get_frame/camera/include/MvCameraControl.h \
    get_frame/camera/include/MvDeviceBase.h \
    get_frame/camera/include/MvErrorDefine.h \
    get_frame/camera/include/MvGigEDevice.h \
    get_frame/camera/include/MvInclude.h \
    get_frame/camera/include/MvUsb3VDevice.h \
    get_frame/camera/include/PixelType.h \
    get_frame/camera/include/TlFactory.h \
    get_frame/debug/debug_settings.h \
    get_frame/frame/frame.h \
    interface/include/CRC_Check.h \
    interface/include/interface.h \
    interface/include/serial.hpp \
    main/global.h \
    watchdog/WatchDog.h \
    algorithm/include/usr_math.h \
    fanwheel/fan_middleware.h \
    fanwheel/fan.hpp \
    fanwheel/fan_param.h \
    main/thread.h


INCLUDEPATH += $$PWD/get_frame/camera/include

