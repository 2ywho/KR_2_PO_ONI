QT += widgets
CONFIG += c++17
CONFIG -= console app_bundle
TARGET = AutopilotLab
SOURCES += main.cpp navigation.cpp uav.cpp sensor.cpp controller.cpp autopilot.cpp
HEADERS += navigation.h uav.h sensor.h controller.h autopilot.h
DEFINES += AUTOPILOT
