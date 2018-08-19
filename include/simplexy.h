//*********************************************************************************************************************************
//
// PROJECT:							astrometry++
// FILE:								simplexy
// SUBSYSTEM:
// LANGUAGE:						C++
// TARGET OS:						none
// LIBRARY DEPENDANCE:	CCfits, Boost, MCL, libAstroClass.
// NAMESPACE:						libAstometry
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GNU GPL V2
//
//                      Copyright 2007 Michael Blanton, Keir Mierle, David W. Hogg, Sam Roweis and Dustin Lang.
//                      Copyright 2013-2014, 2018 Gavin Blakeman.
//                       All rights reserved.
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
// OVERVIEW:            The function simpleXY(...) extracts sources from an image and returns them as an XY list of the sources.
//
// FUNCTIONS INCLUDED:  void simpleXY(...)
//
// CLASSES INCLUDED:    none
//
// CLASS HIERARCHY:     none
//
// HISTORY:             2013-10-31 GGB - Porting the astrometry.net code to C++
//
// TODO:                1. Implement multi-threading on large array manipulations
//
//*********************************************************************************************************************************

/*
 This file is part of the Astrometry.net suite.
 Copyright 2007 Michael Blanton, Keir Mierle, David W. Hogg,
 Sam Roweis and Dustin Lang.

 The Astrometry.net suite is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation, version 2.

 The Astrometry.net suite is distributed in the hope that it will be
 useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with the Astrometry.net suite ; if not, write to the Free
 Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 02110-1301 USA
 */

#ifndef APP_SIMPLEXY_H
#define APP_SIMPLEXY_H

  // Standard libraries

#include <valarray>
#include <vector>

  // astrometry++ header files

#include "config.h"
//#include "dfind2.hpp"
#include "dimage.h"
#include "dmedsmooth.hpp"

  // Miscellaneous Libraries

#include <MCL>
#include <ACL>


namespace APP
{
    // Structure to store the parameters for the simpleXY(...) function.

  struct SSimpleXY
  {
  public:
    FP_t dpsf;          /* gaussian psf width (sigma, not FWHM) */
    FP_t plim;          /* significance to keep */
    FP_t dlim;          /* closest two peaks can be */
    FP_t saddle;        /* saddle difference (in sig) */
    int maxper;         /* maximum number of peaks per object */
    INDEX_t maxnpeaks;  /* maximum number of peaks total */
    int maxsize;        /* maximum size for extended objects */
    int halfbox;        /* size for sliding sky estimation box */
    FP_t sigma;         // If set to non-zero, the given sigma value will be used; otherwise a value will be estimated.

    SSimpleXY();
  };

  /// @brief Searches the image for objects.
  /// @param[in] image - The image to search
  /// @param[in] nx - Size of the image along the x-axis
  /// @param[in] ny - Size of the image along the y-axis.
  /// @param[out] objects - vestor that contains the found sources.
  /// @param[in] nobgsub - Subtract the background before starting the search.
  /// @param[in] parameters - The parameters to use for the search.
  /// @throws None.
  /// @version 2018-05-16/GGB - Converted to a template function.
  /// @version 2013-10-31/GGB - Ported from simplexy_run(...) to C++

  template<typename T>
  void simpleXY(T *image, AXIS_t nx, AXIS_t ny, std::vector<ACL::SImageSource> &objects,
                bool nobgsub, SSimpleXY const &parameters)
  {
    INDEX_t i;
    FP_t limit = 0;
    FP_t sigma = 0;
    INDEX_t npeaks = 0;
    T bgsub[nx * ny];           // background-subtracted image.
    T smoothed[nx * ny];        // PSF-smoothed image.

    std::clog << "simplexy: Begining source extraction..." << std::endl;
    std::clog << "simplexy: x=" << nx << ", y=" << ny << std::endl;
    std::clog << "simplexy: dpsf=" << parameters.dpsf << ", plim=" << parameters.plim << ", dlim=" << parameters.dlim;
    std::clog << ", saddle=" << parameters.saddle << std::endl;
    std::clog << "simplexy: maxper=" << parameters.maxper << ", maxnpeaks=" << parameters.maxnpeaks;
    std::clog << ", maxsize=" << parameters.maxsize << ", halfbox=" << parameters.halfbox << std::endl;

    if (nobgsub)
    {
      for (i = 0; i < static_cast<INDEX_t>(nx * ny); i++)
      {
        bgsub[i] = image[i];
      };
    }
    else
    {
        // background subtraction via median smoothing.

      T medianfiltered[nx * ny];                            // Median filtered image.

      std::clog << "simplexy: median smoothing..." << std::endl;

      dmedsmooth(image, nx, ny, parameters.halfbox, medianfiltered);

        // subtract background from image, placing result in background.
        /// @todo Write this as a multi-threaded function.

      for (i = 0; i < static_cast<INDEX_t>(nx * ny); i++)
      {
        bgsub[i] = image[i] - medianfiltered[i];
      };
    };

      /* smooth by the point spread function (the optimal detection filter, since we assume a symmetric Gaussian PSF) */

    dsmooth(bgsub, nx, ny, parameters.dpsf, smoothed);

      // estimate the noise in the image (sigma)

    if (parameters.sigma == 0.0)
    {
      std::clog << "simplexy: measuring image noise (sigma)..." << std::endl;

      dsigma(image, nx, ny, 5, 0, sigma);

      std::clog << "simplexy: found sigma=" << sigma << std::endl;
    }
    else
    {
      sigma = parameters.sigma;
      std::clog << "simplexy: assuming sigma=" << sigma << std::endl;
    };

      /* The noise in the psf-smoothed image is (approximately) sigma / (2 * sqrt(pi) * dpsf)
       * This ignores the pixelization, replacing the sum by integral. The difference is only significant for small sigma, which
       * would mean your image is undersampled anyway. */

    std::clog << "simplexy: finding objects..." << std::endl;

    limit = (sigma / (2.0 * std::sqrt(MCL::D_PI) * parameters.dpsf)) * parameters.plim;

      /* find pixels above the noise level, and flag a box of pixels around each one. */

    std::valarray<bool> mask(nx * ny);

    dmask(smoothed, nx, ny, limit, parameters.dpsf, mask);

      /* find connected-components in the mask image. */

    label_t nblobs;
    std::valarray<label_t> ccimg(nx * ny);

    dfind2(mask, nx, ny, ccimg, nblobs);

    std::clog << "simplexy: found " << nblobs << " blobs" << std::endl;

      /* find all peaks within each object */

    std::clog << "simplexy: finding peaks..." << std::endl;

    dallpeaks(bgsub, nx, ny, ccimg, objects, npeaks, parameters.dpsf, sigma, parameters.dlim, parameters.saddle,
              parameters.maxper, parameters.maxnpeaks, sigma, parameters.maxsize);

    std::clog << "simplexy: found " << npeaks << " sources." << std::endl;

    for (i = 0; i < npeaks; i++)
    {
      AXIS_t ix = static_cast<AXIS_t>(objects[i].center.x() + 0.5);
      AXIS_t iy = static_cast<AXIS_t>(objects[i].center.y() + 0.5);

      assert(std::isfinite(objects[i].center.x()));
      assert(std::isfinite(objects[i].center.y()));
      assert(ix >= 0);
      assert(iy >= 0);
      assert(ix < nx);
      assert(iy < ny);

      objects[i].flux       = bgsub[ix + iy * nx];
      objects[i].background = image[ix + iy * nx] - objects[i].flux;
    };

    std::clog << "simplexy: Source extraction complete." << std::endl;
  }

} // namespace APP

#endif  // APP_SIMPLEXY_H
