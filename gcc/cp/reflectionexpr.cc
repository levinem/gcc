/* Perform -*- C++ -*- constant expression evaluation, including calls to
   constexpr functions.  These routines are used both during actual parsing
   and during the instantiation of template functions.

   Copyright (C) 1998-2023 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "cp-tree.h"
#include "varasm.h"
#include "c-family/c-objc.h"
#include "tree-iterator.h"
#include "gimplify.h"
#include "builtins.h"
#include "tree-inline.h"
#include "ubsan.h"
#include "timevar.h"
#include "fold-const-call.h"
#include "stor-layout.h"
#include "cgraph.h"
#include "opts.h"
#include "stringpool.h"
#include "attribs.h"
#include "fold-const.h"
#include "intl.h"
#include "toplev.h"

// The expr is the CAST_EXPRESSION from parser.cc.
tree
build_lift_op_expr(location_t loc, tree expr, tsubst_flags_t complain)
{
    // TODO
    tree operand_expr = get_operand_from_unary_expression(expr);
    if (is_valid_lift_op_operand(operand_expr, complain))
    {
        return expr;
    }
    return expr;
}

tree
get_operand_from_unary_expression(tree expr)
{
    // TODO
    return expr;
}

bool
is_valid_lift_op_operand(tree operand_expr, tsubst_flags_t complain)
{
    // TODO
    return true;
}

bool
is_a_type(tree expr, tsubst_flags_t complain)
{
    // TODO
    return true;
}

bool
is_an_expression(tree expr, tsubst_flags_t complain)
{
    // TODO
    return true;
}

bool
is_a_template_name_expression(tree expr, tsubst_flags_t complain)
{
    // TODO
    return true;
}

bool
is_a_namespace_name_expression(tree expr, tsubst_flags_t complain)
{
    // TODO
    return true;
}
