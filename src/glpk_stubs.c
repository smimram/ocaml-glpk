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
    case 0:
      return;

      /*
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
      */

    default:
      caml_raise_constant(*caml_named_value("ocaml_glpk_exn_unknown"));
  }
  assert(0);
}

#define Prob_val(v) (*((glp_prob**)Data_custom_val(v)))

static void finalize_prob(value block)
{
  glp_delete_prob(Prob_val(block));
}

static struct custom_operations prob_ops =
{
  "ocaml_glpk_prob",
  finalize_prob,
  custom_compare_default,
  custom_hash_default,
  custom_serialize_default,
  custom_deserialize_default
};

static value new_prob(glp_prob* lp)
{
  value block = caml_alloc_custom(&prob_ops, sizeof(glp_prob*), 0, 1);
  Prob_val(block) = lp;
  return block;
}

CAMLprim value ocaml_glpk_new_prob(value unit)
{
  glp_prob *lp = glp_create_prob();
  return new_prob(lp);
}

CAMLprim value ocaml_glpk_set_prob_name(value blp, value name)
{
  glp_prob *lp = Prob_val(blp);
  glp_set_prob_name(lp, String_val(name));
  return Val_unit;
}

CAMLprim value ocaml_glpk_get_prob_name(value blp)
{
  CAMLparam1(blp);
  glp_prob *lp = Prob_val(blp);
  CAMLreturn(caml_copy_string(glp_get_prob_name(lp)));
}

CAMLprim value ocaml_glpk_set_obj_name(value blp, value name)
{
  glp_prob *lp = Prob_val(blp);
  glp_set_obj_name(lp, String_val(name));
  return Val_unit;
}

CAMLprim value ocaml_glpk_get_obj_name(value blp)
{
  CAMLparam1(blp);
  glp_prob *lp = Prob_val(blp);
  CAMLreturn(caml_copy_string(glp_get_obj_name(lp)));
}

static int direction_table[] = {GLP_MIN, GLP_MAX};

CAMLprim value ocaml_glpk_set_direction(value blp, value direction)
{
  glp_prob *lp = Prob_val(blp);
  glp_set_obj_dir(lp, direction_table[Int_val(direction)]);
  return Val_unit;
}

CAMLprim value ocaml_glpk_get_direction(value blp)
{
  glp_prob *lp = Prob_val(blp);
  switch(glp_get_obj_dir(lp))
  {
    case GLP_MIN:
      return Val_int(0);

    case GLP_MAX:
      return Val_int(1);

    default:
      assert(0);
  }
}

CAMLprim value ocaml_glpk_add_rows(value blp, value n)
{
  glp_prob *lp = Prob_val(blp);
  glp_add_rows(lp, Int_val(n));
  return Val_unit;
}

CAMLprim value ocaml_glpk_set_row_name(value blp, value n, value name)
{
  glp_prob *lp = Prob_val(blp);
  glp_set_row_name(lp, Int_val(n) + 1, String_val(name));
  return Val_unit;
}

CAMLprim value ocaml_glpk_get_row_name(value blp, value n)
{
  CAMLparam1(blp);
  glp_prob *lp = Prob_val(blp);
  CAMLreturn(caml_copy_string(glp_get_row_name(lp, Int_val(n) + 1)));
}

static int auxvartype_table[] = {GLP_FR, GLP_LO, GLP_UP, GLP_DB, GLP_FX};

CAMLprim value ocaml_glpk_set_row_bounds(value blp, value n, value type, value lb, value ub)
{
  glp_prob *lp = Prob_val(blp);
  glp_set_row_bnds(lp, Int_val(n) + 1, auxvartype_table[Int_val(type)], Double_val(lb), Double_val(ub));
  return Val_unit;
}

CAMLprim value ocaml_glpk_add_cols(value blp, value n)
{
  glp_prob *lp = Prob_val(blp);
  glp_add_cols(lp, Int_val(n));
  return Val_unit;
}

CAMLprim value ocaml_glpk_set_col_name(value blp, value n, value name)
{
  glp_prob *lp = Prob_val(blp);
  glp_set_col_name(lp, Int_val(n) + 1, String_val(name));
  return Val_unit;
}

CAMLprim value ocaml_glpk_get_col_name(value blp, value n)
{
  CAMLparam1(blp);
  glp_prob *lp = Prob_val(blp);
  CAMLreturn(caml_copy_string(glp_get_col_name(lp, Int_val(n) + 1)));
}

CAMLprim value ocaml_glpk_set_col_bounds(value blp, value n, value type, value lb, value ub)
{
  glp_prob *lp = Prob_val(blp);
  glp_set_col_bnds(lp, Int_val(n) + 1, auxvartype_table[Int_val(type)], Double_val(lb), Double_val(ub));
  return Val_unit;
}

CAMLprim value ocaml_glpk_set_obj_coef(value blp, value n, value coef)
{
  glp_prob *lp = Prob_val(blp);
  glp_set_obj_coef(lp, Int_val(n) + 1, Double_val(coef));
  return Val_unit;
}

CAMLprim value ocaml_glpk_load_matrix(value blp, value matrix)
{
  glp_prob *lp = Prob_val(blp);
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
  glp_load_matrix(lp, n - 1, ia, ja, ar);

  free(ia);
  free(ja);
  free(ar);

  return Val_unit;
}

CAMLprim value ocaml_glpk_load_sparse_matrix(value blp, value matrix)
{
  glp_prob *lp = Prob_val(blp);
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
  glp_load_matrix(lp, len, ia, ja, ar);

  free(ia);
  free(ja);
  free(ar);

  return Val_unit;
}

CAMLprim value ocaml_glpk_simplex(value blp)
{
  CAMLparam1(blp);
  glp_prob *lp = Prob_val(blp);
  int ret;

  caml_enter_blocking_section();
  ret = glp_simplex(lp, NULL); // TODO: control parameters
  caml_leave_blocking_section();

  raise_on_error(ret);
  CAMLreturn(Val_unit);
}

CAMLprim value ocaml_glpk_get_obj_val(value blp)
{
  glp_prob *lp = Prob_val(blp);
  return caml_copy_double(glp_get_obj_val(lp));
}

CAMLprim value ocaml_glpk_mip_obj_val(value blp)
{
  glp_prob *lp = Prob_val(blp);
  return caml_copy_double(glp_mip_obj_val(lp));
}

CAMLprim value ocaml_glpk_get_col_prim(value blp, value n)
{
  glp_prob *lp = Prob_val(blp);
  return caml_copy_double(glp_get_col_prim(lp, Int_val(n) + 1));
}

CAMLprim value ocaml_glpk_get_row_prim(value blp, value n)
{
  glp_prob *lp = Prob_val(blp);
  return caml_copy_double(glp_get_row_prim(lp, Int_val(n) + 1));
}

CAMLprim value ocaml_glpk_get_row_dual(value blp, value n)
{
  glp_prob *lp = Prob_val(blp);
  return caml_copy_double(glp_get_row_dual(lp, Int_val(n) + 1));
}

CAMLprim value ocaml_glpk_get_num_rows(value blp)
{
  glp_prob *lp = Prob_val(blp);
  return Val_int(glp_get_num_rows(lp));
}

CAMLprim value ocaml_glpk_get_num_cols(value blp)
{
  glp_prob *lp = Prob_val(blp);
  return Val_int(glp_get_num_cols(lp));
}

CAMLprim value ocaml_glpk_scale_problem(value blp)
{
  glp_prob *lp = Prob_val(blp);
  glp_scale_prob(lp, GLP_SF_AUTO); // TODO: scaling options
  return Val_unit;
}

CAMLprim value ocaml_glpk_unscale_problem(value blp)
{
  glp_prob *lp = Prob_val(blp);
  glp_unscale_prob(lp);
  return Val_unit;
}

CAMLprim value ocaml_glpk_interior(value blp)
{
  CAMLparam1(blp);
  glp_prob *lp = Prob_val(blp);
  int ret;

  caml_enter_blocking_section();
  ret = glp_interior(lp, NULL); //TODO: parameters
  caml_leave_blocking_section();

  raise_on_error(ret);
  CAMLreturn(Val_unit);
}

CAMLprim value ocaml_glpk_warm_up(value blp)
{
  glp_prob *lp = Prob_val(blp);
  raise_on_error(glp_warm_up(lp));
  return Val_unit;
}

static int kind_table[] = {GLP_CV, GLP_IV, GLP_BV};

CAMLprim value ocaml_glpk_set_col_kind(value blp, value n, value kind)
{
  glp_prob *lp = Prob_val(blp);
  glp_set_col_kind(lp, Int_val(n) + 1, kind_table[Int_val(kind)]);
  return Val_unit;
}

CAMLprim value ocaml_glpk_intopt(value blp)
{
  CAMLparam1(blp);
  glp_prob *lp = Prob_val(blp);
  int ret;

  caml_enter_blocking_section();
  ret = glp_intopt(lp, NULL); // TODO: parameters
  caml_leave_blocking_section();

  raise_on_error(ret);
  CAMLreturn(Val_unit);
}

static int status_table[] = {GLP_OPT, GLP_FEAS, GLP_INFEAS, GLP_NOFEAS, GLP_UNBND, GLP_UNDEF};

static int status_int (int n){
  int i=0;
  while (status_table[i] != n) i++;
  return i;
}

CAMLprim value ocaml_glpk_mip_status(value blp)
{
  CAMLparam1(blp);
  glp_prob *lp = Prob_val(blp);
  CAMLreturn(Val_int(status_int(glp_mip_status(lp))));
}

/*
#define BIND_INT_PARAM(name, param) \
CAMLprim value ocaml_glpk_get_##name(value blp) \
{ \
  glp_prob *lp = Prob_val(blp); \
  return Val_int(glp_get_int_parm(lp, param)); \
} \
CAMLprim value ocaml_glpk_set_##name(value blp, value n) \
{ \
  glp_prob *lp = Prob_val(blp); \
  glp_set_int_parm(lp, param, Int_val(n)); \
  return Val_unit; \
}
*/

/* BIND_INT_PARAM(message_level, glp_prob_K_MSGLEV); */
/* BIND_INT_PARAM(scaling, glp_prob_K_SCALE); */
/* BIND_INT_PARAM(use_dual_simplex, glp_prob_K_DUAL); */
/* BIND_INT_PARAM(pricing, glp_prob_K_PRICE); */
/* BIND_REAL_PARAM(relaxation, glp_prob_K_RELAX); */
/* BIND_INT_PARAM(solution_rounding, glp_prob_K_ROUND); */
/* BIND_INT_PARAM(iteration_limit, glp_prob_K_ITLIM); */
/* BIND_INT_PARAM(iteration_count, glp_prob_K_ITCNT); */
/* BIND_REAL_PARAM(time_limit, glp_prob_K_TMLIM); */
/* BIND_INT_PARAM(branching_heuristic, glp_prob_K_BRANCH); */
/* BIND_INT_PARAM(backtracking_heuristic, glp_prob_K_BTRACK); */
/* BIND_INT_PARAM(use_presolver, glp_prob_K_PRESOL); */

CAMLprim value ocaml_glpk_read_cplex(value fname)
{
  
  glp_prob *lp = glp_create_prob();
  int ret = glp_read_lp(lp, NULL, String_val(fname));
  if (!ret) caml_failwith("Error while reading data in CPLEX LP format.");
  return new_prob(lp);
}

CAMLprim value ocaml_glpk_write_cplex(value blp, value fname)
{
  if (glp_write_lp(Prob_val(blp), NULL, String_val(fname)))
    caml_failwith("Error while writing data in CPLEX LP format.");
  return Val_unit;
}
