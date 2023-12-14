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

    tree parsed_operand_expr = parse_lift_op_expr_operand(operand_expr, complain);

    if (is_valid_lift_op_operand(parsed_operand_expr, complain))
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

tree
parse_lift_op_expr_operand(tree operand_expr, tsubst_flags_t complain)
{
    // Tentatively parse a template-name.
    tree parsed_template_expr = parse_template_name_operand(operand_expr, complain);
    if (is_valid_parsed_template_expr(parsed_template_expr)) {
        return act_on_reflected_template(parsed_template_expr, complain);
    }

    // Otherwise, check for the global namespace
    if (is_the_current_token_a_colon_colon(operand_expr) && is_the_next_token_a_right_parenthesize(operand_expr)) {
        reflected_namespace_expr = consume_the_colon_colon_token(operand_expr);
        return act_on_reflected_namespace(stripped_operand_expr, complain);
    }

    // Otherwise, parse a namespace-name.
    if (is_namespace_name(operand_expr))
    {
        tree parsed_namespace_name_expr = parse_namespace_name_expr(operand_expr);
        return act_on_reflected_namespace(parsed_namespace_name_expr);
    }

}

bool
is_namespace_name(tree operand_expr)
{
    return true;
}

tree
parse_namespace_name_expr(operand_expr)
{
    return operand_expr;
}

bool
is_the_current_token_a_colon_colon(tree operand_expr)
{
    return true;
}

bool
is_the_next_token_a_right_parenthesize(tree operand_expr)
{
    return true;
}

bool
is_valid_parsed_template_expr(tree parsed_template_expr, tsubst_flags_t complain)
{
    return true;
}

tree
parse_template_name_operand(operand_expr, complain)
{
    return operand_expr;
}

bool
is_valid_lift_op_operand(tree operand_expr, tsubst_flags_t complain)
{
    if (
        is_a_template_name_expression ()
       )
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
