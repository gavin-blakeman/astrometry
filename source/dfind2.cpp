//*********************************************************************************************************************************
//
// PROJECT:							astrometry++
// FILE:								dfind
// SUBSYSTEM:
// LANGUAGE:						C++
// TARGET OS:						none
// LIBRARY DEPENDANCE:	ACL, Boost, MCL.
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
// HISTORY:             2013-10-31 GGB - Porting the astrometry.net code to C++
//
//*********************************************************************************************************************************

/*
  This file is part of the Astrometry.net suite.
  Copyright 2006, 2007 Michael Blanton, Keir Mierle, David W. Hogg, Sam Roweis
  and Dustin Lang.

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
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <valarray>
#include <vector>

  // APP header files.

#include "../include/config.h"
#include "../include/error.h"

  // Miscellaneous libraries

#include <SCL>

namespace APP
{

/*
 * This code does connected component analysis, but instead of returning a list
 * of components, it does the following crazy thing: it returns an image where
 * each component is all numbered the same. For example, if your input is:
 *
 *  . . . . . . . . .
 *  . . . 1 . . . . .
 *  . . 1 1 1 . . . .
 *  . . . 1 . . . . .
 *  . . . . . . . . .
 *  . 1 1 . . . 1 . .
 *  . 1 1 . . 1 1 1 .
 *  . 1 1 . . . 1 . .
 *  . . . . . . . . .
 *
 * where . is 0. Then your output is:
 *
 *  . . . . . . . . .
 *  . . . 0 . . . . .
 *  . . 0 0 0 . . . .
 *  . . . 0 . . . . .
 *  . . . . . . . . .
 *  . 1 1 . . . 2 . .
 *  . 1 1 . . 2 2 2 .
 *  . 1 1 . . . 2 . .
 *  . . . . . . . . .
 *
 * where . is now -1. Diagonals are considered connections, so the following is a single component:
 *  . . . . .
 *  . 1 . 1 .
 *  . . 1 . .
 *  . 1 . 1 .
 *  . . . . .
 */

  /// @brief Finds the root of this set (which is the min label) but collapses intermediate labels as it goes. */
  /// @param[in] label:
  /// @param[out] equivs:
  /// @version 2014-01-26/GGB - Converted to C++
  /// @version 2007/Keir Mierle
  /// @version 2006-01/Mike Blanton

  label_t collapsing_find_minlabel(label_t label, std::vector<label_t> &equivs)
  {
    label_t min = label;

    while (equivs[min] != min)
    {
      min = equivs[min];
    };

    while (label != min)
    {
      label_t next = equivs[label];
      equivs[label] = min;
      label = next;
    };

    return min;
  }

  /// @brief Relabels the image.
  /// @param[in] on-pixels:
  /// @param[in] maxlabel:
  /// @param[out] equivs:
  /// @param[out] object:
  /// @version 2014-01-26/GGB - Converted to C++
  /// @version 2007/Keir Mierle
  /// @version 2006-01/Mike Blanton

  label_t relabel_image(std::vector<INDEX_t> const &on_pixels, label_t maxlabel, std::vector<label_t> &equivs,
                        std::valarray<label_t> &object)
  {
    size_t i;
    label_t maxcontiguouslabel = 0;
    std::valarray<label_t> number(maxlabel);

    for (i = 0; i < static_cast<size_t>(maxlabel); i++)
    {
      number[i] = std::numeric_limits<label_t>::max();
    };

    for (i = 0; i < on_pixels.size(); i++)
    {
      INDEX_t onpix;
      label_t minlabel;
      onpix = on_pixels[i];
      minlabel = collapsing_find_minlabel(object[onpix], equivs);

      if (number[minlabel] == std::numeric_limits<label_t>::max())
      {
        number[minlabel] = maxcontiguouslabel++;
      };
      object[onpix] = number[minlabel];
    };

    return maxcontiguouslabel;
  }

  /// @brief Find non-zero objects in a binary image.
  /// @param[in] image: The image to search.
  /// @param[in] nx: Number of pixels in the x-axis
  /// @param[in] ny: Number of pixels in the y-axis.
  /// @param[in] object:
  /// @param[out] nobjects: The number of objects found.
  /// @throws 0xA200 - astrometry++::dfind::dfind2(): Ran out of labels.
  /// @version 2013-10-31/GGB - Converted from astrometry.net to C++
  /// @version 2007/Keir Mierle
  /// @version 2006-01/Mike Blanton

  void dfind2(std::valarray<bool> const &image, AXIS_t nx, AXIS_t ny, std::valarray<label_t> &object, label_t &nobjects)
  {
    AXIS_t ix, iy;
    INDEX_t i;
    std::vector<label_t> equivs;
    label_t maxlabel = 0;

      /* Keep track of 'on' pixels to avoid later rescanning */

    std::vector<INDEX_t> on_pixels;

      /* Find blobs and track equivalences */

    for (iy = 0; iy < ny; iy++)
    {
      for (ix = 0; ix < nx; ix++)
      {
        label_t thislabel, thislabelmin;

        object[nx * iy + ix] = -1;

        if (image[nx * iy + ix])
        {
            /* Store location of each 'on' pixel. */

          on_pixels.push_back(nx * iy + ix);

            /* If the pixel to the left [exists and] is on, this pixel joins its group. */

          if (ix && image[nx * iy + ix - 1])
          {
              /* Old group */

            object[nx * iy + ix] = object[nx * iy + ix - 1];
          }
          else
          {
              /* New blob */
              // FIXME this part should become uf_new_group()

            object[nx * iy + ix] = maxlabel;
            equivs.push_back(maxlabel);
            maxlabel++;

            if (maxlabel == std::numeric_limits<label_t>::max())
            {
                // This is exceptionally unlikely with a int_32.

              std::clog << "Ran out of labels.  Attempting to relabel..." << std::endl;
              maxlabel = relabel_image(on_pixels, maxlabel, equivs, object);
              std::clog << "After relabelling, we need " << maxlabel << " labels" << std::endl;
              if (maxlabel == std::numeric_limits<label_t>::max())
              {
                std::clog << "Ran out of labels." << std::endl;
                APP_ERROR(0xA200);  // astrometry++::dfind::dfind2(): Ran out of labels.
              };
            };
          };

          thislabel  = object[nx * iy + ix];

            /* Compute minimum equivalence label for this pixel */

          thislabelmin = collapsing_find_minlabel(thislabel, equivs);

          if (iy != 0)
          {
              /* Check three pixels above this one which are 'neighbours' */

            for (i = std::max(0, ix - 1); i <= std::min(ix + 1, nx - 1); i++)
            {
              if (image[nx * (iy - 1) + i])
              {
                label_t otherlabel = object[nx * (iy - 1) + i];

                  /* Find min of the other */

                label_t otherlabelmin = collapsing_find_minlabel(otherlabel, equivs);

                  /* Merge groups if necessary */

                if (thislabelmin != otherlabelmin)
                {
                  label_t oldlabelmin = std::max(thislabelmin, otherlabelmin);
                  label_t newlabelmin = std::min(thislabelmin, otherlabelmin);
                  thislabelmin = newlabelmin;
                  equivs[oldlabelmin] = newlabelmin;
                  equivs[thislabel] = newlabelmin;

                    /* Update other pixel too */
                  object[nx * (iy - 1) + i] = newlabelmin;
                };
              };
            };
          };
          object[nx*iy + ix] = thislabelmin;
        }
      }
    }

      /* Re-label the groups before returning */

    maxlabel = relabel_image(on_pixels, maxlabel, equivs, object);

    std::clog << "After final relabelling, " << maxlabel << " labels were used." << std::endl;

    nobjects = maxlabel;

  }

}  // namespace APP
