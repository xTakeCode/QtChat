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

QT += network
QT += widgets
QT += multimedia

RESOURCES += images.qrc

FORMS   += MainWindow.ui

HEADERS += MainWindow.h \
           Sound.h \
           Emoticons.h

SOURCES += MainWindow.cxx \
           main.cpp \
           Sound.cpp \
           Emoticons.cpp
