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
#include <caml/memory.h>
#include <caml/misc.h>
#include <caml/mlvalues.h>
#include <caml/fail.h>
#include <caml/callback.h>

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

/*
static LPX* lpx_of_block(value block)
{
  return (LPX*)Field(block, 1);
}
*/
#define lpx_of_block(block) (LPX*)Field(block, 1)

static void finalize_lpx(value block)
{
  LPX *lp = lpx_of_block(block);
  lpx_delete_prob(lp);
}

CAMLprim value ocaml_glpk_new_prob(value unit)
{
  CAMLparam1(unit);
  CAMLlocal1(block);
  LPX *lp = lpx_create_prob();
  block = caml_alloc_final(2, finalize_lpx, 150, 1024);
  /* Store_field(block, 1, (value)lp); */
  /* Cf. http://pauillac.inria.fr/caml/caml-list/2320.html */
  Field(block, 1) = (value)lp;
  CAMLreturn(block);
}

CAMLprim value ocaml_glpk_set_prob_name(value blp, value name)
{
  CAMLparam2(blp, name);
  LPX *lp = lpx_of_block(blp);
  lpx_set_prob_name(lp, String_val(name));
  CAMLreturn(Val_unit);
}

CAMLprim value ocaml_glpk_get_prob_name(value blp)
{
  CAMLparam1(blp);
  LPX *lp = lpx_of_block(blp);
  CAMLreturn(caml_copy_string(lpx_get_prob_name(lp)));
}

CAMLprim value ocaml_glpk_set_obj_name(value blp, value name)
{
  CAMLparam2(blp, name);
  LPX *lp = lpx_of_block(blp);
  lpx_set_obj_name(lp, String_val(name));
  CAMLreturn(Val_unit);
}

CAMLprim value ocaml_glpk_get_obj_name(value blp)
{
  CAMLparam1(blp);
  LPX *lp = lpx_of_block(blp);
  CAMLreturn(caml_copy_string(lpx_get_obj_name(lp)));
}

static int direction_table[] = {LPX_MIN, LPX_MAX};

CAMLprim value ocaml_glpk_set_direction(value blp, value direction)
{
  CAMLparam2(blp, direction);
  LPX *lp = lpx_of_block(blp);
  lpx_set_obj_dir(lp, direction_table[Int_val(direction)]);
  CAMLreturn(Val_unit);
}

CAMLprim value ocaml_glpk_get_direction(value blp)
{
  CAMLparam1(blp);
  LPX *lp = lpx_of_block(blp);
  switch(lpx_get_obj_dir(lp))
  {
    case LPX_MIN:
      CAMLreturn(Val_int(0));

    case LPX_MAX:
      CAMLreturn(Val_int(1));

    default:
      break;
  }
  assert(0);
  CAMLreturn(Val_int(-1));
}

CAMLprim value ocaml_glpk_add_rows(value blp, value n)
{
  CAMLparam2(blp, n);
  LPX *lp = lpx_of_block(blp);
  lpx_add_rows(lp, Int_val(n));
  CAMLreturn(Val_unit);
}

CAMLprim value ocaml_glpk_set_row_name(value blp, value n, value name)
{
  CAMLparam3(blp, n, name);
  LPX *lp = lpx_of_block(blp);
  lpx_set_row_name(lp, Int_val(n) + 1, String_val(name));
  CAMLreturn(Val_unit);
}

CAMLprim value ocaml_glpk_get_row_name(value blp, value n)
{
  CAMLparam2(blp, n);
  LPX *lp = lpx_of_block(blp);
  CAMLreturn(caml_copy_string(lpx_get_row_name(lp, Int_val(n) + 1)));
}

static int auxvartype_table[] = {LPX_FR, LPX_LO, LPX_UP, LPX_DB, LPX_FX};

CAMLprim value ocaml_glpk_set_row_bounds(value blp, value n, value type, value lb, value ub)
{
  CAMLparam5(blp, n, type, lb, ub);
  LPX *lp = lpx_of_block(blp);
  lpx_set_row_bnds(lp, Int_val(n) + 1, auxvartype_table[Int_val(type)], Double_val(lb), Double_val(ub));
  CAMLreturn(Val_unit);
}

CAMLprim value ocaml_glpk_add_cols(value blp, value n)
{
  CAMLparam2(blp, n);
  LPX *lp = lpx_of_block(blp);
  lpx_add_cols(lp, Int_val(n));
  CAMLreturn(Val_unit);
}

CAMLprim value ocaml_glpk_set_col_name(value blp, value n, value name)
{
  CAMLparam3(blp, n, name);
  LPX *lp = lpx_of_block(blp);
  lpx_set_col_name(lp, Int_val(n) + 1, String_val(name));
  CAMLreturn(Val_unit);
}

CAMLprim value ocaml_glpk_get_col_name(value blp, value n)
{
  CAMLparam2(blp, n);
  LPX *lp = lpx_of_block(blp);
  CAMLreturn(caml_copy_string(lpx_get_col_name(lp, Int_val(n) + 1)));
}

CAMLprim value ocaml_glpk_set_col_bounds(value blp, value n, value type, value lb, value ub)
{
  CAMLparam5(blp, n, type, lb, ub);
  LPX *lp = lpx_of_block(blp);
  lpx_set_col_bnds(lp, Int_val(n) + 1, auxvartype_table[Int_val(type)], Double_val(lb), Double_val(ub));
  CAMLreturn(Val_unit);
}

CAMLprim value ocaml_glpk_set_obj_coef(value blp, value n, value coef)
{
  CAMLparam3(blp, n, coef);
  LPX *lp = lpx_of_block(blp);
  lpx_set_obj_coef(lp, Int_val(n) + 1, Double_val(coef));
  CAMLreturn(Val_unit);
}

CAMLprim value ocaml_glpk_load_matrix(value blp, value matrix)
{
  CAMLparam2(blp, matrix);
  LPX *lp = lpx_of_block(blp);
  int i_dim = Wosize_val(matrix), j_dim;
  int *ia, *ja;
  double *ar;
  double x;
  int i, j, n;

  if (i_dim <= 0)
    CAMLreturn(Val_unit);
  j_dim = Wosize_val(Field(matrix, 0)) / 2;
  ia = (int*)malloc((i_dim * j_dim + 1) * sizeof(int));
  ja = (int*)malloc((i_dim * j_dim + 1) * sizeof(int));
  ar = (double*)malloc((i_dim * j_dim + 1) * sizeof(double));
  n = 1;
  for(i = 0; i < i_dim; i++)
  {
    /* TODO: raise an error */
    assert(Wosize_val(Field(matrix, i)) == j_dim * 2);
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

  CAMLreturn(Val_unit);
}

CAMLprim value ocaml_glpk_simplex(value blp)
{
  CAMLparam1(blp);
  LPX *lp = lpx_of_block(blp);
  raise_on_error(lpx_simplex(lp));
  CAMLreturn(Val_unit);
}

CAMLprim value ocaml_glpk_get_obj_val(value blp)
{
  CAMLparam1(blp);
  LPX *lp = lpx_of_block(blp);
  double ans = lpx_get_obj_val(lp);
  CAMLreturn(caml_copy_double(ans));
}

CAMLprim value ocaml_glpk_get_col_prim(value blp, value n)
{
  CAMLparam2(blp, n);
  LPX *lp = lpx_of_block(blp);
  double ans = lpx_get_col_prim(lp, Int_val(n) + 1);
  CAMLreturn(caml_copy_double(ans));
}

CAMLprim value ocaml_glpk_get_num_rows(value blp)
{
  CAMLparam1(blp);
  LPX *lp = lpx_of_block(blp);
  int ans = lpx_get_num_rows(lp);
  CAMLreturn(Val_int(ans));
}

CAMLprim value ocaml_glpk_get_num_cols(value blp)
{
  CAMLparam1(blp);
  LPX *lp = lpx_of_block(blp);
  int ans = lpx_get_num_cols(lp);
  CAMLreturn(Val_int(ans));
}

CAMLprim value ocaml_glpk_scale_problem(value blp)
{
  CAMLparam1(blp);
  LPX *lp = lpx_of_block(blp);
  lpx_scale_prob(lp);
  CAMLreturn(Val_unit);
}

CAMLprim value ocaml_glpk_unscale_problem(value blp)
{
  CAMLparam1(blp);
  LPX *lp = lpx_of_block(blp);
  lpx_unscale_prob(lp);
  CAMLreturn(Val_unit);
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
  LPX *lp = lpx_of_block(blp);
  raise_on_error(lpx_interior(lp));
  CAMLreturn(Val_unit);
}

static int class_table[] = {LPX_LP, LPX_MIP};

CAMLprim value ocaml_glpk_set_class(value blp, value class)
{
  CAMLparam2(blp, class);
  LPX *lp = lpx_of_block(blp);
  lpx_set_class(lp, class_table[Int_val(class)]);
  CAMLreturn(Val_unit);
}

CAMLprim value ocaml_glpk_get_class(value blp)
{
  CAMLparam1(blp);
  LPX *lp = lpx_of_block(blp);
  switch(lpx_get_class(lp))
  {
    case LPX_LP:
      CAMLreturn(Val_int(0));

    case LPX_MIP:
      CAMLreturn(Val_int(1));

    default:
      break;
  }
  assert(0);
  CAMLreturn(Val_int(-1));
}

static int kind_table[] = {LPX_CV, LPX_IV};

CAMLprim value ocaml_glpk_set_col_kind(value blp, value n, value kind)
{
  CAMLparam3(blp, n, kind);
  LPX *lp = lpx_of_block(blp);
  lpx_set_col_kind(lp, Int_val(n) + 1, kind_table[Int_val(kind)]);
  CAMLreturn(Val_unit);
}

CAMLprim value ocaml_glpk_integer(value blp)
{
  CAMLparam1(blp);
  LPX *lp = lpx_of_block(blp);
  raise_on_error(lpx_integer(lp));
  CAMLreturn(Val_unit);
}

CAMLprim value ocaml_glpk_set_message_level(value blp, value level)
{
  CAMLparam2(blp, level);
  LPX *lp = lpx_of_block(blp);
  if (Int_val(level) < 0 && Int_val(level) > 3)
    caml_invalid_argument("level");
  lpx_set_int_parm(lp, LPX_K_MSGLEV, Int_val(level));
  CAMLreturn(Val_unit);
}

CAMLprim value ocaml_glpk_warm_up(value blp)
{
  CAMLparam1(blp);
  LPX *lp = lpx_of_block(blp);
  raise_on_error(lpx_warm_up(lp));
  CAMLreturn(Val_unit);
}

CAMLprim value ocaml_glpk_use_presolver(value blp, value b)
{
  CAMLparam2(blp, b);
  LPX *lp = lpx_of_block(blp);
  lpx_set_int_parm(lp, LPX_K_PRESOL, Int_val(b));
  CAMLreturn(Val_unit);
}
