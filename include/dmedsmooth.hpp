//*********************************************************************************************************************************
//
// PROJECT:							astrometry++
// FILE:								dmedsmooth
// SUBSYSTEM:
// LANGUAGE:						C++
// TARGET OS:						none
// LIBRARY DEPENDANCE:	MCL, SCL.
// NAMESPACE:						libAstometry
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GNU GPL V2
//
//                      Copyright 2006, 2007 Michael Blanton, Keir Mierle, David W. Hogg, Sam Roweis and Dustin Lang.
//                      Copyright 2013-2014, 2018 Gavin Blakeman.
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
// HISTORY:             2013-11-01 GGB - Porting the astrometry.net code to C++
//
// TODO:
//
//*********************************************************************************************************************************

/*
  This file is part of the Astrometry.net suite.
  Copyright 2006, 2007 Michael Blanton, Keir Mierle, David W. Hogg,
  Sam Roweis and Dustin Lang.

  The Astrometry.net suite is free software; you can redistribute
  it and/or modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation, version 2.

  The Astrometry.net suite is distributed in the hope that it will be
  useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the Astrometry.net suite ; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
*/

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <valarray>


  // Storage Class Library

//#include <SCL>

namespace APP
{

  /// @brief Median smooth an image -- actually, compute median values for a grid of points, then interpolate.
  /// @param[in] image: The image array.
  /// @param[in] nx: Extent of the x-axis
  /// @param[in] ny: Extent of the y-axis
  /// @param[in] halfbox:
  /// @param[out] smooth: Pointer to an array to store the smoothed data.
  /// @throws None
  /// @version 2013-11-01/GGB - Ported to C++
  /// @version 2006-01/Mike Blanton - Function created.

  template<typename T>
  void dmedsmooth(std::valarray<T> const &image, AXIS_t nx, AXIS_t ny, AXIS_t halfbox, std::valarray<T> &smooth)
  {
    AXIS_t i, j, ip, jp, ist, jst, nb, ind, jnd, sp;
    AXIS_t xoff, yoff, nm, nxgrid, nygrid;
    AXIS_t ypsize, ymsize, xpsize, xmsize;
    FP_t dx, dy, xkernel, ykernel;

      /* get grids */

    sp = halfbox;
    nxgrid = nx / sp + 2;

      // "xgrid" are the centers.
      // "xlo" are the (inclusive) lower-bounds
      // "xhi" are the (inclusive) upper-bounds
      // the grid cells may overlap.

    std::valarray<T> xgrid(nxgrid);
    std::valarray<T> xlo(nxgrid);
    std::valarray<T> xhi(nxgrid);

    xoff = (nx - 1 - (nxgrid - 3) * sp) / 2;

    for (i = 1; i < (nxgrid - 1); i++)
    {
      xgrid[i] = (i - 1) * sp + xoff;
    };

    xgrid[0] = xgrid[1] - sp;
    xgrid[nxgrid - 1] = xgrid[nxgrid - 2] + sp;

    for (i = 0; i < nxgrid; i++)
    {
      xlo[i] = std::max(xgrid[i] - sp, 0);
      xhi[i] = std::min(xgrid[i] + sp, nx-1);
    }

    nygrid = ny / sp + 2;

    T ylo[nygrid];
    T yhi[nygrid];
    T ygrid[nygrid];

    yoff = (ny - 1 - (nygrid - 3) * sp) / 2;
    for (i = 1; i < nygrid - 1; i++)
    {
      ygrid[i] = (i - 1) * sp + yoff;
    };

    ygrid[0] = ygrid[1] - sp;
    ygrid[nygrid - 1] = ygrid[nygrid - 2] + sp;

    for (i = 0; i < nygrid; i++)
    {
      ylo[i] = std::max(ygrid[i] - sp, 0);
      yhi[i] = std::min(ygrid[i] + sp, ny-1);
    };

      // the median-filtered image (subsampled on a grid).

    std::valarray<T> grid(nxgrid * nygrid);

    std::vector<T> arr((sp * 2 + 5) * (sp * 2 + 5));

    for (j = 0; j < nygrid; j++)
    {
      for (i = 0; i < nxgrid; i++)
      {
        nb = 0;
        for (jp = ylo[j]; jp <= yhi[j]; jp++)
        {
          for (ip = xlo[i]; ip <= xhi[i]; ip++)
          {
            arr[nb] = image[ip + jp * nx];
            nb++;
          };
        };

        if (nb > 1)
        {
          nm = nb / 2;
          std::sort(std::begin(arr), std::end(arr));        // Note this used to call dselip
          grid[i + j * nxgrid] = arr[nm];
        }
        else
        {
          grid[i + j*nxgrid] = image[static_cast<INDEX_t>(xlo[i]) + static_cast<INDEX_t>(ylo[j]) * nx];
        };
      };
    };

    for (j = 0; j < ny; j++)
    {
      for (i = 0; i < nx; i++)
      {
        smooth[i + j*nx] = 0.;
      };
    };

    for (j = 0;j < nygrid;j++)
    {
      jst = static_cast<AXIS_t>(static_cast<FP_t>(ygrid[j]) - static_cast<FP_t>(sp) * 1.5);
      jnd = static_cast<AXIS_t>(static_cast<FP_t>(ygrid[j]) + static_cast<FP_t>(sp) * 1.5);

      if (jst < 0)
      {
        jst = 0;
      };

      if (jnd > ny - 1)
      {
        jnd = ny - 1;
      };

      ypsize = sp;
      ymsize = sp;

      if (j == 0)
      {
        ypsize = ygrid[1] - ygrid[0];
      };

      if (j == 1)
      {
        ymsize = ygrid[1] - ygrid[0];
      };

      if (j == nygrid - 2)
      {
        ypsize = ygrid[nygrid - 1] - ygrid[nygrid - 2];
      };

      if (j == nygrid - 1)
      {
        ymsize = ygrid[nygrid - 1] - ygrid[nygrid - 2];
      };

      for (i = 0;i < nxgrid;i++)
      {
        ist = static_cast<AXIS_t>(static_cast<FP_t>(xgrid[i]) - static_cast<FP_t>(sp) * 1.5);
        ind = static_cast<AXIS_t>(static_cast<FP_t>(xgrid[i]) + static_cast<FP_t>(sp) * 1.5);

        if (ist < 0)
        {
          ist = 0;
        };

        if (ind > nx - 1)
        {
          ind = nx - 1;
        };
        xpsize = sp;
        xmsize = sp;

        if (i == 0)
        {
          xpsize = xgrid[1] - xgrid[0];
        };

        if (i == 1)
        {
          xmsize = xgrid[1] - xgrid[0];
        };

        if (i == nxgrid - 2)
        {
          xpsize = xgrid[nxgrid - 1] - xgrid[nxgrid - 2];
        };

        if (i == nxgrid - 1)
        {
          xmsize = xgrid[nxgrid - 1] - xgrid[nxgrid - 2];
        };

        for (jp = jst; jp <= jnd; jp++)
        {
          dy = static_cast<FP_t>(jp - ygrid[j]);
          ykernel = 0.;

          if (dy > -1.5*ymsize && dy <= -0.5*ymsize)
          {
            ykernel = 0.5 * (dy / ymsize + 1.5) * (dy / ymsize + 1.5);
          }
          else if (dy > -0.5*ymsize && dy < 0.)
          {
            ykernel = -(dy * dy / ymsize / ymsize - 0.75);
          }
          else if (dy < 0.5*ypsize && dy >= 0.)
          {
            ykernel = -(dy * dy / ypsize / ypsize - 0.75);
          }
          else if (dy >= 0.5*ypsize && dy < 1.5*ypsize)
          {
            ykernel = 0.5 * (dy / ypsize - 1.5) * (dy / ypsize - 1.5);
          };

          for (ip = ist;ip <= ind;ip++)
          {
            dx = static_cast<FP_t>(ip - xgrid[i]);
            xkernel = 0.;

            if (dx > -1.5*xmsize && dx <= -0.5*xmsize)
            {
              xkernel = 0.5 * (dx / xmsize + 1.5) * (dx / xmsize + 1.5);
            }
            else if (dx > -0.5*xmsize && dx < 0.)
            {
              xkernel = -(dx * dx / xmsize / xmsize - 0.75);
            }
            else if (dx < 0.5*xpsize && dx >= 0.)
            {
              xkernel = -(dx * dx / xpsize / xpsize - 0.75);
            }
            else if (dx >= 0.5*xpsize && dx < 1.5*xpsize)
            {
              xkernel = 0.5 * (dx / xpsize - 1.5) * (dx / xpsize - 1.5);
            };

            smooth[ip + jp*nx] += xkernel * ykernel * grid[i + j * nxgrid];
          };
        };
      };
    };
  }

} // namespace APP
