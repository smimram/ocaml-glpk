/*
 * ocaml-glpk - OCaml bindings to glpk
 * Copyright (C) 2004 Samuel Mimram
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/* $Id$ */

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/custom.h>
#include <caml/fail.h>
#include <caml/memory.h>
#include <caml/misc.h>
#include <caml/mlvalues.h>
#include <caml/signals.h>

#include <assert.h>

#include <glpk.h>

static void raise_on_error(int ret)
{
  switch(ret)
  {
    case LPX_E_OK:
      return;

    case LPX_E_FAULT:
      caml_raise_constant(*caml_named_value("ocaml_glpk_exn_fault"));

    case LPX_E_OBJLL:
      caml_raise_constant(*caml_named_value("ocaml_glpk_exn_objll"));

    case LPX_E_OBJUL:
      caml_raise_constant(*caml_named_value("ocaml_glpk_exn_objul"));

    case LPX_E_NOPFS:
      caml_raise_constant(*caml_named_value("ocaml_glpk_exn_nopfs"));

    case LPX_E_NODFS:
      caml_raise_constant(*caml_named_value("ocaml_glpk_exn_nodfs"));

    case LPX_E_ITLIM:
      caml_raise_constant(*caml_named_value("ocaml_glpk_exn_itlim"));

    case LPX_E_TMLIM:
      caml_raise_constant(*caml_named_value("ocaml_glpk_exn_tmlim"));

    case LPX_E_SING:
      caml_raise_constant(*caml_named_value("ocaml_glpk_exn_sing"));

    case LPX_E_EMPTY:
      caml_raise_constant(*caml_named_value("ocaml_glpk_exn_empty"));

    case LPX_E_BADB:
      caml_raise_constant(*caml_named_value("ocaml_glpk_exn_badb"));

    case LPX_E_NOCONV:
      caml_raise_constant(*caml_named_value("ocaml_glpk_exn_noconv"));

    default:
      caml_raise_constant(*caml_named_value("ocaml_glpk_exn_unknown"));
  }
  assert(0); /* TODO */
}

#define Lpx_val(v) (*((LPX**)Data_custom_val(v)))

static void finalize_lpx(value block)
{
  lpx_delete_prob(Lpx_val(block));
}

static struct custom_operations lpx_ops =
{
  "ocaml_glpk_lpx",
  finalize_lpx,
  custom_compare_default,
  custom_hash_default,
  custom_serialize_default,
  custom_deserialize_default
};

static value new_blp(LPX* lp)
{
  value block = caml_alloc_custom(&lpx_ops, sizeof(LPX*), 0, 1);
  Lpx_val(block) = lp;
  return block;
}

CAMLprim value ocaml_glpk_new_prob(value unit)
{
  LPX *lp = lpx_create_prob();
  return new_blp(lp);
}

CAMLprim value ocaml_glpk_set_prob_name(value blp, value name)
{
  LPX *lp = Lpx_val(blp);
  lpx_set_prob_name(lp, String_val(name));
  return Val_unit;
}

CAMLprim value ocaml_glpk_get_prob_name(value blp)
{
  CAMLparam1(blp);
  LPX *lp = Lpx_val(blp);
  CAMLreturn(caml_copy_string(lpx_get_prob_name(lp)));
}

CAMLprim value ocaml_glpk_set_obj_name(value blp, value name)
{
  LPX *lp = Lpx_val(blp);
  lpx_set_obj_name(lp, String_val(name));
  return Val_unit;
}

CAMLprim value ocaml_glpk_get_obj_name(value blp)
{
  CAMLparam1(blp);
  LPX *lp = Lpx_val(blp);
  CAMLreturn(caml_copy_string(lpx_get_obj_name(lp)));
}

static int direction_table[] = {LPX_MIN, LPX_MAX};

CAMLprim value ocaml_glpk_set_direction(value blp, value direction)
{
  LPX *lp = Lpx_val(blp);
  lpx_set_obj_dir(lp, direction_table[Int_val(direction)]);
  return Val_unit;
}

CAMLprim value ocaml_glpk_get_direction(value blp)
{
  LPX *lp = Lpx_val(blp);
  switch(lpx_get_obj_dir(lp))
  {
    case LPX_MIN:
      return Val_int(0);

    case LPX_MAX:
      return Val_int(1);

    default:
      assert(0);
  }
}

CAMLprim value ocaml_glpk_add_rows(value blp, value n)
{
  LPX *lp = Lpx_val(blp);
  lpx_add_rows(lp, Int_val(n));
  return Val_unit;
}

CAMLprim value ocaml_glpk_set_row_name(value blp, value n, value name)
{
  LPX *lp = Lpx_val(blp);
  lpx_set_row_name(lp, Int_val(n) + 1, String_val(name));
  return Val_unit;
}

CAMLprim value ocaml_glpk_get_row_name(value blp, value n)
{
  CAMLparam1(blp);
  LPX *lp = Lpx_val(blp);
  CAMLreturn(caml_copy_string(lpx_get_row_name(lp, Int_val(n) + 1)));
}

static int auxvartype_table[] = {LPX_FR, LPX_LO, LPX_UP, LPX_DB, LPX_FX};

CAMLprim value ocaml_glpk_set_row_bounds(value blp, value n, value type, value lb, value ub)
{
  LPX *lp = Lpx_val(blp);
  lpx_set_row_bnds(lp, Int_val(n) + 1, auxvartype_table[Int_val(type)], Double_val(lb), Double_val(ub));
  return Val_unit;
}

CAMLprim value ocaml_glpk_add_cols(value blp, value n)
{
  LPX *lp = Lpx_val(blp);
  lpx_add_cols(lp, Int_val(n));
  return Val_unit;
}

CAMLprim value ocaml_glpk_set_col_name(value blp, value n, value name)
{
  LPX *lp = Lpx_val(blp);
  lpx_set_col_name(lp, Int_val(n) + 1, String_val(name));
  return Val_unit;
}

CAMLprim value ocaml_glpk_get_col_name(value blp, value n)
{
  CAMLparam1(blp);
  LPX *lp = Lpx_val(blp);
  CAMLreturn(caml_copy_string(lpx_get_col_name(lp, Int_val(n) + 1)));
}

CAMLprim value ocaml_glpk_set_col_bounds(value blp, value n, value type, value lb, value ub)
{
  LPX *lp = Lpx_val(blp);
  lpx_set_col_bnds(lp, Int_val(n) + 1, auxvartype_table[Int_val(type)], Double_val(lb), Double_val(ub));
  return Val_unit;
}

CAMLprim value ocaml_glpk_set_obj_coef(value blp, value n, value coef)
{
  LPX *lp = Lpx_val(blp);
  lpx_set_obj_coef(lp, Int_val(n) + 1, Double_val(coef));
  return Val_unit;
}

CAMLprim value ocaml_glpk_load_matrix(value blp, value matrix)
{
  LPX *lp = Lpx_val(blp);
  int i_dim = Wosize_val(matrix), j_dim;
  int *ia, *ja;
  double *ar;
  double x;
  int i, j, n;

  if (i_dim <= 0)
    return Val_unit;

  j_dim = Wosize_val(Field(matrix, 0)) / Double_wosize;
  ia = (int*)malloc((i_dim * j_dim + 1) * sizeof(int));
  ja = (int*)malloc((i_dim * j_dim + 1) * sizeof(int));
  ar = (double*)malloc((i_dim * j_dim + 1) * sizeof(double));
  n = 1;

  for(i = 0; i < i_dim; i++)
  {
    /* TODO: raise an error */
    assert(Wosize_val(Field(matrix, i)) / Double_wosize == j_dim);
    for(j = 0; j < j_dim; j++)
    {
      x = Double_field(Field(matrix, i), j);
      /* We only want non null elements. */
      if (x != 0)
      {
        ia[n] = i + 1;
        ja[n] = j + 1;
        ar[n] = x;
        n++;
      }
    }
  }
  lpx_load_matrix(lp, n - 1, ia, ja, ar);

  free(ia);
  free(ja);
  free(ar);

  return Val_unit;
}

CAMLprim value ocaml_glpk_load_sparse_matrix(value blp, value matrix)
{
  LPX *lp = Lpx_val(blp);
  int len = Wosize_val(matrix);
  int *ia, *ja;
  double *ar;
  int i;
  value e;

  ia = (int*)malloc((len + 1) * sizeof(int));
  ja = (int*)malloc((len + 1) * sizeof(int));
  ar = (double*)malloc((len + 1) * sizeof(double));

  for(i = 0; i < len; i++)
  {
    e = Field(matrix, i);
    ia[i+1] = Int_val(Field(Field(e, 0), 0)) + 1;
    ja[i+1] = Int_val(Field(Field(e, 0), 1)) + 1;
    ar[i+1] = Double_val(Field(e, 1));
  }
  lpx_load_matrix(lp, len, ia, ja, ar);

  free(ia);
  free(ja);
  free(ar);

  return Val_unit;
}

CAMLprim value ocaml_glpk_simplex(value blp)
{
  CAMLparam1(blp);
  LPX *lp = Lpx_val(blp);
  int ret;

  caml_enter_blocking_section();
  ret = lpx_simplex(lp);
  caml_leave_blocking_section();

  raise_on_error(ret);
  CAMLreturn(Val_unit);
}

CAMLprim value ocaml_glpk_get_obj_val(value blp)
{
  LPX *lp = Lpx_val(blp);
  double ans;
  if (lpx_get_class(lp) == LPX_MIP)
    ans = lpx_mip_obj_val(lp);
  else
    ans = lpx_get_obj_val(lp);
  return caml_copy_double(ans);
}

CAMLprim value ocaml_glpk_get_col_prim(value blp, value n)
{
  LPX *lp = Lpx_val(blp);
  double ans;
  /* TODO: is it the right thing to do? */
  if (lpx_get_class(lp) == LPX_MIP)
    ans = lpx_mip_col_val(lp, Int_val(n) + 1);
  else
    ans = lpx_get_col_prim(lp, Int_val(n) + 1);
  return caml_copy_double(ans);
}

CAMLprim value ocaml_glpk_get_row_prim(value blp, value n)
{
  LPX *lp = Lpx_val(blp);
  return caml_copy_double(lpx_get_row_prim(lp, Int_val(n) + 1));
}

CAMLprim value ocaml_glpk_get_row_dual(value blp, value n)
{
  LPX *lp = Lpx_val(blp);
  return caml_copy_double(lpx_get_row_dual(lp, Int_val(n) + 1));
}

CAMLprim value ocaml_glpk_get_num_rows(value blp)
{
  LPX *lp = Lpx_val(blp);
  return Val_int(lpx_get_num_rows(lp));
}

CAMLprim value ocaml_glpk_get_num_cols(value blp)
{
  LPX *lp = Lpx_val(blp);
  return Val_int(lpx_get_num_cols(lp));
}

CAMLprim value ocaml_glpk_scale_problem(value blp)
{
  LPX *lp = Lpx_val(blp);
  lpx_scale_prob(lp);
  return Val_unit;
}

CAMLprim value ocaml_glpk_unscale_problem(value blp)
{
  LPX *lp = Lpx_val(blp);
  lpx_unscale_prob(lp);
  return Val_unit;
}

/* TODO */
/*
CAMLprim value ocaml_glpk_check_kkt(value blp, value scaled, value vkkt)
{

}
*/

CAMLprim value ocaml_glpk_interior(value blp)
{
  CAMLparam1(blp);
  LPX *lp = Lpx_val(blp);
  int ret;

  caml_enter_blocking_section();
  ret = lpx_interior(lp);
  caml_leave_blocking_section();

  raise_on_error(ret);
  CAMLreturn(Val_unit);
}

static int class_table[] = {LPX_LP, LPX_MIP};

CAMLprim value ocaml_glpk_set_class(value blp, value class)
{
  LPX *lp = Lpx_val(blp);
  lpx_set_class(lp, class_table[Int_val(class)]);
  return Val_unit;
}

CAMLprim value ocaml_glpk_get_class(value blp)
{
  LPX *lp = Lpx_val(blp);
  switch(lpx_get_class(lp))
  {
    case LPX_LP:
      return Val_int(0);

    case LPX_MIP:
      return Val_int(1);

    default:
      assert(0);
  }
}

static int kind_table[] = {LPX_CV, LPX_IV};

CAMLprim value ocaml_glpk_set_col_kind(value blp, value n, value kind)
{
  LPX *lp = Lpx_val(blp);
  lpx_set_col_kind(lp, Int_val(n) + 1, kind_table[Int_val(kind)]);
  return Val_unit;
}

CAMLprim value ocaml_glpk_integer(value blp)
{
  CAMLparam1(blp);
  LPX *lp = Lpx_val(blp);
  int ret;

  caml_enter_blocking_section();
  ret = lpx_integer(lp);
  caml_leave_blocking_section();

  raise_on_error(ret);
  CAMLreturn(Val_unit);
}

CAMLprim value ocaml_glpk_intopt(value blp)
{
  CAMLparam1(blp);
  LPX *lp = Lpx_val(blp);
  int ret;

  caml_enter_blocking_section();
  ret = lpx_intopt(lp);
  caml_leave_blocking_section();

  raise_on_error(ret);
  CAMLreturn(Val_unit);
}

CAMLprim value ocaml_glpk_warm_up(value blp)
{
  LPX *lp = Lpx_val(blp);
  raise_on_error(lpx_warm_up(lp));
  return Val_unit;
}

#define BIND_INT_PARAM(name, param) \
CAMLprim value ocaml_glpk_get_##name(value blp) \
{ \
  LPX *lp = Lpx_val(blp); \
  return Val_int(lpx_get_int_parm(lp, param)); \
} \
CAMLprim value ocaml_glpk_set_##name(value blp, value n) \
{ \
  LPX *lp = Lpx_val(blp); \
  lpx_set_int_parm(lp, param, Int_val(n)); \
  return Val_unit; \
}

#define BIND_REAL_PARAM(name, param) \
CAMLprim value ocaml_glpk_get_##name(value blp) \
{ \
  LPX *lp = Lpx_val(blp); \
  double ans = lpx_get_real_parm(lp, param); \
  return caml_copy_double(ans); \
} \
CAMLprim value ocaml_glpk_set_##name(value blp, value n) \
{ \
  LPX *lp = Lpx_val(blp); \
  lpx_set_real_parm(lp, param, Double_val(n)); \
  return Val_unit; \
}

BIND_INT_PARAM(message_level, LPX_K_MSGLEV);
BIND_INT_PARAM(scaling, LPX_K_SCALE);
BIND_INT_PARAM(use_dual_simplex, LPX_K_DUAL);
BIND_INT_PARAM(pricing, LPX_K_PRICE);
BIND_REAL_PARAM(relaxation, LPX_K_RELAX);
/*
BIND_REAL_PARAM(relative_tolerance, LPX_K_TOLBND);
BIND_REAL_PARAM(absolute_tolerance, LPX_K_TOLDJ);
*/
BIND_INT_PARAM(solution_rounding, LPX_K_ROUND);
BIND_INT_PARAM(iteration_limit, LPX_K_ITLIM);
BIND_INT_PARAM(iteration_count, LPX_K_ITCNT);
BIND_REAL_PARAM(time_limit, LPX_K_TMLIM);
BIND_INT_PARAM(branching_heuristic, LPX_K_BRANCH);
BIND_INT_PARAM(backtracking_heuristic, LPX_K_BTRACK);
BIND_INT_PARAM(use_presolver, LPX_K_PRESOL);

CAMLprim value ocaml_glpk_read_cplex(value fname)
{
  LPX *lp = lpx_read_cpxlp(String_val(fname));
  if (!lp)
    caml_failwith("Error while reading data in CPLEX LP format.");
  return new_blp(lp);
}

CAMLprim value ocaml_glpk_write_cplex(value blp, value fname)
{
  if (lpx_write_cpxlp(Lpx_val(blp), String_val(fname)))
    caml_failwith("Error while writing data in CPLEX LP format.");
  return Val_unit;
}
