// Copyright (C) 2019-2025 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

// { dg-do compile { target c++20 } }

#include <algorithm>
#include <array>

constexpr bool
test()
{
  constexpr std::array<int, 12> ca0{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}};
  constexpr std::array<int, 3> cam{{4, 5, 6}};
  constexpr std::array<int, 3> camm{{-4, -5, -6}};

  const auto outf = std::find_end(ca0.begin(), ca0.end(),
				  cam.begin(), cam.end());

  const auto outg = std::find_end(ca0.begin(), ca0.end(),
				  camm.begin(), camm.end(),
				  [](int i, int j){ return i + 1 == -j; });

  return outf == (ca0.begin() + 4) && outg == (ca0.begin() + 3);
}

static_assert(test());
