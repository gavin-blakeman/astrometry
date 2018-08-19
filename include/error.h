//*********************************************************************************************************************************
//
// PROJECT:							astrometry++
// FILE:								error
// SUBSYSTEM:						Error subsystem
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	None.
// NAMESPACE:						libAstrometry
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2011-2013, 2018 Gavin Blakeman.
//                      All rights reserved.
//
//                      This file is part of the astrometry++ suite, a C++ port of the astrometry.net code. This port has been
//                      developed as a library to support the astroManager (www.astroManager.net) project.
//
//                      The astrometry++ suite is free software: you can redistribute it and/or modify it under the terms of the
//                      GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or
//                      (at your option) any later version.
//
//                      The astrometry++ suite is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
//                      even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
//                      Public License for more details.
//
//                      You should have received a copy of the GNU General Public License along with the astrometry++ suite.  If
//                      not, see <http://www.gnu.org/licenses/>.
//
// PROJECT OVERVIEW:    This is a port of the astrometry.net software into the C++ programming language. The port includes
//                      the conversion to C++ as well as the extension of the software to support x64 processors and the extended
//                      addressing.
//
// CLASSES INCLUDED:
//
// HISTORY:             2013-30-31 GGB - File Created
//
//*********************************************************************************************************************************

#ifndef APP_ERROR_H
#define APP_ERROR_H

  // Miscellaneous libraries

#include <GCL>

namespace APP
{

#define APP_ERROR(ERR) (ERROR(APP, ERR))
#define APP_CODE_ERROR CODE_ERROR(APP)

}	// namespace APP

#endif // APP_ERROR_H
