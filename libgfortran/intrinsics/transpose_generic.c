/* Implementation of the TRANSPOSE intrinsic
   Copyright 2003 Free Software Foundation, Inc.
   Contributed by Tobias Schl?ter

This file is part of the GNU Fortran 95 runtime library (libgfortran).

Libgfortran is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

In addition to the permissions in the GNU General Public License, the
Free Software Foundation gives you unlimited permission to link the
compiled version of this file into combinations with other programs,
and to distribute those combinations without any restriction coming
from the use of this file.  (The General Public License restrictions
do apply in other respects; for example, they cover modification of
the file, and distribution when not linked into a combine
executable.)

Libgfortran is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public
License along with libgfortran; see the file COPYING.  If not,
write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#include "config.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "libgfortran.h"

extern void transpose (gfc_array_char *, gfc_array_char *);
export_proto(transpose);

static void
transpose_internal (gfc_array_char *ret, gfc_array_char *source,
		    index_type size)
{
  /* r.* indicates the return array.  */
  index_type rxstride, rystride;
  char *rptr;
  /* s.* indicates the source array.  */
  index_type sxstride, systride;
  const char *sptr;

  index_type xcount, ycount;
  index_type x, y;

  assert (GFC_DESCRIPTOR_RANK (source) == 2
          && GFC_DESCRIPTOR_RANK (ret) == 2);

  if (ret->data == NULL)
    {
      assert (ret->dtype == source->dtype);

      ret->dim[0].lbound = 0;
      ret->dim[0].ubound = source->dim[1].ubound - source->dim[1].lbound;
      ret->dim[0].stride = 1;

      ret->dim[1].lbound = 0;
      ret->dim[1].ubound = source->dim[0].ubound - source->dim[0].lbound;
      ret->dim[1].stride = ret->dim[0].ubound+1;

      ret->data = internal_malloc_size (size * size0 ((array_t*)ret));
      ret->offset = 0;
    }

  sxstride = source->dim[0].stride * size;
  if (sxstride == 0)
    sxstride = size;
  systride = source->dim[1].stride * size;
  xcount = source->dim[0].ubound + 1 - source->dim[0].lbound;
  ycount = source->dim[1].ubound + 1 - source->dim[1].lbound;

  rxstride = ret->dim[0].stride * size;
  if (rxstride == 0)
    rxstride = size;
  rystride = ret->dim[1].stride * size;

  rptr = ret->data;
  sptr = source->data;

  for (y = 0; y < ycount; y++)
    {
      for (x = 0; x < xcount; x++)
        {
          memcpy (rptr, sptr, size);

          sptr += sxstride;
          rptr += rystride;
        }
      sptr += systride - (sxstride * xcount);
      rptr += rxstride - (rystride * xcount);
    }
}

extern void transpose (gfc_array_char *, gfc_array_char *);
export_proto(transpose);

void
transpose (gfc_array_char *ret, gfc_array_char *source)
{
  transpose_internal (ret, source, GFC_DESCRIPTOR_SIZE (source));
}

extern void transpose_char (gfc_array_char *, GFC_INTEGER_4,
			    gfc_array_char *, GFC_INTEGER_4);
export_proto(transpose_char);

void
transpose_char (gfc_array_char *ret,
		GFC_INTEGER_4 ret_length __attribute__((unused)),
		gfc_array_char *source, GFC_INTEGER_4 source_length)
{
  transpose_internal (ret, source, source_length);
}
