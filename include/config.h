//*********************************************************************************************************************************
//
// PROJECT:							astrometry++
// FILE:								config
// SUBSYSTEM:
// LANGUAGE:						C++
// TARGET OS:						none
// LIBRARY DEPENDANCE:	CCfits, Boost, MCL, libAstroClass.
// NAMESPACE:						libAstometry
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GNU GPL V2
//
//                      Copyright 2013, 2018 Gavin Blakeman.
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
// PROJECT OVERVIEW:    This is a port of the astrometry.net software into the C++ programming language.
//
// OVERVIEW:
//
// CLASSES INCLUDED:    none
//
// CLASS HIERARCHY:     none
//
// HISTORY:             2013-11-01 GGB - File created.
//
//*********************************************************************************************************************************

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <cstdint>

namespace APP
{
  typedef double FP_t;

  typedef std::int32_t AXIS_t;         // Type used for axis indexes
  typedef size_t INDEX_t;               // Type used for array indexes.
  typedef size_t NODE_t;                // Type used for nodes.

  typedef std::int32_t label_t;

  extern size_t maxThreads;

} // namespace APP

#endif // APP_CONFIG_H
