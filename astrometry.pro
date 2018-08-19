#-----------------------------------------------------------------------------------------------------------------------------------
#
# PROJECT:            astrometry Library
# FILE:								astrometry.pro
# SUBSYSTEM:          Project File
# LANGUAGE:						C++
# TARGET OS:          WINDOWS/UNIX/LINUX/MAC
# LIBRARY DEPENDANCE:	None.
# NAMESPACE:          N/A
# AUTHOR:							Gavin Blakeman.
# LICENSE:            GPLv2
#
#                     Copyright 2014-2018 Gavin Blakeman.
#                     This file is part of the astrometry Library
#
#                     astrometry is free software: you can redistribute it and/or modify it under the terms of the GNU General
#                     Public License as published by the Free Software Foundation, either version 2 of the License, or (at your
#                     option) any later version.
#
#                     astrometry is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
#                     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
#                     License for more details.
#
#                     You should have received a copy of the GNU General Public License along with WCL.  If not, see
#                     <http://www.gnu.org/licenses/>.
#
# OVERVIEW:						Project file for compiling the project
#
# HISTORY:            2014-02-23/GGB - Development of classes for astroManager
#
#-----------------------------------------------------------------------------------------------------------------------------------

TARGET = astrometry
TEMPLATE = lib
CONFIG += staticlib

QT       -= core gui

QMAKE_CXXFLAGS += -std=c++17 -static -static-libgcc
DEFINES += BOOST_THREAD_USE_LIB

win32:CONFIG(release, debug|release) {
  DESTDIR = "../Library/win32/release"
  OBJECTS_DIR = "../Library/win32/release/object/ACL"
}
else:win32:CONFIG(debug, debug|release) {
  DESTDIR = "../Library/win32/debug"
  OBJECTS_DIR = "../Library/win32/debug/object/ACL"
}
else:unix:CONFIG(release, debug|release) {
  DESTDIR = ""
  OBJECTS_DIR = "objects"
}
else:unix:CONFIG(debug, debug|release) {
  DESTDIR = ""
  OBJECTS_DIR = "objects"
}

INCLUDEPATH += \
  "../ACL" \
  "../boost 1.62" \
  "../cfitsio" \
  "../dlib-19.4" \
  "../GCL" \
  "../libWCS/wcstools-3.8.7/libwcs" \
  "../MCL" \
  "../NOVAS" \
  "../PCL" \
  "../SBIG" \
  "../SCL" \
  "../SOFA/src" \
  "../"

HEADERS += \
    include/simplexy.h \
    include/config.h \
    include/dmedsmooth.hpp \
    include/error.h \
    include/dimage.h


SOURCES += \
    source/error.cpp \
    source/dfind2.cpp

OTHER_FILES +=

DISTFILES += \
    LICENSE
