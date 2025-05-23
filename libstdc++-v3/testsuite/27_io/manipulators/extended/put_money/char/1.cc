// { dg-do run { target c++11 } }
// { dg-require-namedlocale "de_DE.ISO8859-15" }

// 2010-03-01  Paolo Carlini  <paolo.carlini@oracle.com>

// Copyright (C) 2010-2025 Free Software Foundation, Inc.
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

#include <sstream>
#include <iomanip>
#include <testsuite_hooks.h>

void test01()
{
  std::locale loc_de = std::locale(ISO_8859(15,de_DE));

  std::ostringstream oss;
  oss.imbue(loc_de);

  const std::string str("720000000000");
  oss << std::put_money(str);

  VERIFY( oss.str() == "7.200.000.000,00 " );
  VERIFY( oss.good() );
}

int main()
{
  test01();
  return 0;
}
