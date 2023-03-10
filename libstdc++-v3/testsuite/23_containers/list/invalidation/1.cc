// List iterator invalidation tests

// Copyright (C) 2003, 2004, 2005 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
// USA.

#include <debug/list>
#include <iterator>
#include <testsuite_hooks.h>

using debug::list;
using std::advance;

bool test = true;

// Assignment
void test01()
{
  list<int> v1;
  list<int> v2;

  v1.push_front(17);

  list<int>::iterator start = v1.begin();
  list<int>::iterator finish = v1.end();
  VERIFY(start._M_dereferenceable());
  VERIFY(!finish._M_dereferenceable() && !finish._M_singular());

  v1 = v2;
  VERIFY(start._M_singular());
  VERIFY(!finish._M_dereferenceable() && !finish._M_singular());

  finish = v1.end();
  v1.assign(v2.begin(), v2.end());
  VERIFY(!finish._M_dereferenceable() && !finish._M_singular());

  finish = v1.end();
  v1.assign(17, 42);
  VERIFY(!finish._M_dereferenceable() && !finish._M_singular());
}

int main()
{
  test01();
  return 0;
}
