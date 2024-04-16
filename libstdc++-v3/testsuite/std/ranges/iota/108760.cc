// Copyright (C) 2020-2024 Free Software Foundation, Inc.
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

// Fixes https://gcc.gnu.org/bugzilla/show_bug.cgi?id=108760
// { dg-do run { target c++23 } }

#include <numeric>
#include <testsuite_hooks.h>

const int ARR_SIZE = 4;

void
test01()
{
    int expected_arr[ARR_SIZE] = {0, 1, 2, 3};
    int input_arr[ARR_SIZE] = {0, 0, 0, 0};
    std::ranges::iota(input_arr, 0);
    for (int i = 0; i < ARR_SIZE; i++) {
        VERIFY( input_arr[i] == expected_arr[i]);
    }
}

int
main()
{
    test01();
}
