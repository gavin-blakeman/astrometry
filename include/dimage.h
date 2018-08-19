//*********************************************************************************************************************************
//
// PROJECT:							astrometry++
// FILE:								dimage
// SUBSYSTEM:
// LANGUAGE:						C++
// TARGET OS:						none
// LIBRARY DEPENDANCE:	CCfits, Boost, MCL, libAstroClass.
// NAMESPACE:						libAstometry
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GNU GPL V2
//
//                      Copyright 2006, 2007 Michael Blanton, Keir Mierle, David W. Hogg, Sam Roweis and Dustin Lang.
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
// PROJECT OVERVIEW:    This is a port of the astrometry.net software into the C++ programming language. The port includes
//                      the conversion to C++ as well as the extension of the software to support x64 processors and the extended
//                      addressing.
//
// OVERVIEW:
//
// CLASSES INCLUDED:    none
//
// CLASS HIERARCHY:     none
//
// HISTORY:             2013-10-31 GGB - Porting the astrometry.net code to C++
//
//*********************************************************************************************************************************

/*
  This file is part of the Astrometry.net suite.
  Copyright 2006, 2007 Michael Blanton, Keir Mierle, David W. Hogg,
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

#ifndef APP_DIMAGE_H
#define APP_DIMAGE_H

// Standard library includes

#include <cstdint>
#include <valarray>
#include <vector>

#include "config.h"
#include "simplexy.h"

  // Boost Library includes

#include "boost/scoped_array.hpp"
#include "boost/shared_array.hpp"

#include <MCL>

namespace APP
{
  void dfind2(std::valarray<bool> const &, AXIS_t, AXIS_t, std::valarray<label_t> &, label_t &);
//  int dfluxes(double *image, double *templates, double *weights, int nx, int ny,
//              double *xcen, double *ycen, int nchild, double *children, double sigma);
//  int dweights(double *image, double *invvar, int nx, int ny, int ntemplates,
//               double *templates, int nonneg, double *weights);

//  void dsmooth(std::valarray<FP_t> image, AXIS_t nx, AXIS_t ny, FP_t sigma, std::valarray<FP_t> smooth);

///*
// int dobjects(double *image, double *smooth, int nx, int ny,
// double dpsf, double plim, int *objects);
// */
//  bool dobjects(double *image, int nx, int ny, double limit, double dpsf, int *objects);

//int dnonneg(double *xx, double *invcovar, double *bb, double offset,
//            int nn, double tolerance, int maxiter, int *niter, double *chi2,
//            int verbose);
//int dcentral(double *image, int nx, int ny, int npeaks, double *xcen,
//             double *ycen, int *central, double sigma, double dlim,
//             double saddle, int maxnpeaks);
//int dmsmooth(double *image, int nx, int ny, int box, double *smooth);
//int deblend(double *image,
//            double *invvar,
//            int nx,
//            int ny,
//            int *nchild,
//            int *xcen,
//            int *ycen,
//            double *cimages,
//            double *templates,
//            double sigma,
//            double dlim,
//            double tsmooth,   /* smoothing of template */
//            double tlimit,    /* lowest template value in units of sigma */
//            double tfloor,    /* vals < tlimit*sigma are set to tfloor*sigma */
//            double saddle,    /* number of sigma for allowed saddle */
//            double parallel,  /* how parallel you allow templates to be */
//            int maxnchild,
//            double minpeak,
//            int starstart,
//            double *psf,
//            int pnx,
//            int pny,
//            int dontsettemplates);


//int dtemplates(double *image, int nx, int ny, int *ntemplates, int *xcen,
//               int *ycen, double *templates, double sigma, double parallel);

} // namespace APP

#endif // APP_DIMAGE_H
