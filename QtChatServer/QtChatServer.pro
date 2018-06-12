######################################
#                                    #
#           QtChat © 2018            #
#             Author:                #
#          Rafal Olszewski           #
#         github.com/T8kme/          #
#                                    #
######################################

TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .

CONFIG += console
CONFIG += qt

QT += network
QT += widgets
QT += sql

HEADERS += \
    DbManager.h \
    QtChatServer.h
SOURCES += main.cpp \
    DbManager.cpp \
    QtChatServer.cpp
