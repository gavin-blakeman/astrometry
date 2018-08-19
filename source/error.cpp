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
//                      All rights reserved
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
// CLASSES INCLUDED:    CLoader
//
// HISTORY:             2013-30-31 GGB - File Created
//
//*********************************************************************************************************************************

#include "../include/error.h"

  // Standard C++ library headers

#include <string>
#include <vector>

namespace APP
{
  class CLoader
  {
  private:
    void loadErrorMessages();

  public:
    CLoader();
  };

  static CLoader executeLoader();

  CLoader::CLoader()
  {
    loadErrorMessages();
  }


  void CLoader::loadErrorMessages()
  {
    std::vector<std::pair<GCL::TErrorCode, std::string>> errors =
    {
      {0x0001, "astrometry++::dmask(): Failed to find any significant pixels."},
      {0xA000, "libAstrometry::starxy: Index outside of allowed range. ((index < 0) || (index >= assignedSize))."},
      {0xA001, "libAstrometry::starxy: Item at index is null, unable to return value."},
      {0xA100, "libAstrometry::quadfile: Failed to open quads file."},
      {0xA101, "libAstrometry::quadfile::CQuadFile constructor: Cannot pass nullptr as FITS filename."},
      {0xA102, "libAstrometry::quadfile::CQuadFile constructor: Error while opening/creating FITS file."},
      {0xA103, "libAstrometry::quadfile::CQuadFile::check(): dimquads has illegal value."},
      {0xA104, "libAstrometry::quadfile::CQuadFile::check(): Failed to get quad."},
      {0xA105, "libAstrometry::quadfile::CQuadFile::check(): Star ID is out of bounds."},
      {0xA106, "libAstrometry::quadfile::CQuadFile::getStars(): Requested quad ID out of range."},
      {0xA200, "astrometry++::dfind::dfind2(): Ran out of labels."},
    };

    std::for_each(errors.begin(), errors.end(),
                  [] (std::pair<GCL::TErrorCode, std::string> p) { GCL::CError::addErrorMessage("APP", p.first, p.second); });
  }


}  // namespace APP
