(*
 * ocaml-glpk - OCaml bindings to glpk
 * Copyright (C) 2004-2019 Samuel Mimram
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
 *)

type lp

type direction = Minimize | Maximize

type aux_var_type = Free_var | Lower_bounded_var | Upper_bounded_var | Double_bounded_var | Fixed_var

type prob_class = Linear_prog | Mixed_integer_prog

type var_kind = Continuous_var | Integer_var | Boolean_var

type status = Optimal | Feasible | Infeasible | No_feasible | Unbounded | Undefined

exception Unknown_error of int

let _ =
  Callback.register_exception "ocaml_glpk_exn_unkown" (Unknown_error 0)

external new_problem : unit -> lp = "ocaml_glpk_new_prob"

external set_prob_name : lp -> string -> unit = "ocaml_glpk_set_prob_name"

external get_prob_name : lp -> string = "ocaml_glpk_get_prob_name"

external set_obj_name : lp -> string -> unit = "ocaml_glpk_set_obj_name"

external get_obj_name : lp -> string = "ocaml_glpk_get_obj_name"

external set_direction : lp -> direction -> unit = "ocaml_glpk_set_direction"

external get_direction : lp -> direction = "ocaml_glpk_get_direction"

external add_rows : lp -> int -> unit = "ocaml_glpk_add_rows"

external set_row_name : lp -> int -> string -> unit = "ocaml_glpk_set_row_name"

external get_row_name : lp -> int -> string = "ocaml_glpk_get_row_name"

external set_row_bounds : lp -> int -> aux_var_type -> float -> float -> unit = "ocaml_glpk_set_row_bounds"

external add_columns : lp -> int -> unit = "ocaml_glpk_add_cols"

external set_col_name : lp -> int -> string -> unit = "ocaml_glpk_set_col_name"

external get_col_name : lp -> int -> string = "ocaml_glpk_get_col_name"

external set_col_bounds : lp -> int -> aux_var_type -> float -> float -> unit = "ocaml_glpk_set_col_bounds"

external set_obj_coef : lp -> int -> float -> unit = "ocaml_glpk_set_obj_coef"

external load_matrix : lp -> float array array -> unit = "ocaml_glpk_load_matrix"

external load_sparse_matrix : lp -> ((int * int) * float) array -> unit = "ocaml_glpk_load_sparse_matrix"

external simplex : lp -> unit = "ocaml_glpk_simplex"

external get_obj_val : lp -> float = "ocaml_glpk_get_obj_val"

external get_col_primal : lp -> int -> float = "ocaml_glpk_get_col_prim"

external get_row_primal : lp -> int -> float = "ocaml_glpk_get_row_prim"

external get_row_dual : lp -> int -> float = "ocaml_glpk_get_row_dual"

let make_problem dir zcoefs constr pbounds xbounds =
  let lp = new_problem () in
    set_direction lp dir;
    add_rows lp (Array.length pbounds);
    for i = 0 to (Array.length pbounds) - 1 do
      match pbounds.(i) with
        | lb, ub when lb = -.infinity && ub = infinity -> set_row_bounds lp i Free_var 0. 0.
        | lb, ub when ub = infinity -> set_row_bounds lp i Lower_bounded_var lb 0.
        | lb, ub when lb = -.infinity -> set_row_bounds lp i Upper_bounded_var 0. ub
        | lb, ub when lb = ub -> set_row_bounds lp i Fixed_var lb ub
        | lb, ub -> set_row_bounds lp i Double_bounded_var lb ub
    done;
    add_columns lp (Array.length xbounds);
    for i = 0 to (Array.length xbounds) - 1 do
      set_obj_coef lp i zcoefs.(i);
      match xbounds.(i) with
        | lb, ub when lb = -.infinity && ub = infinity -> set_col_bounds lp i Free_var 0. 0.
        | lb, ub when ub = infinity -> set_col_bounds lp i Lower_bounded_var lb 0.
        | lb, ub when lb = -.infinity -> set_col_bounds lp i Upper_bounded_var 0. ub
        | lb, ub when lb = ub -> set_col_bounds lp i Fixed_var lb ub
        | lb, ub -> set_col_bounds lp i Double_bounded_var lb ub
    done;
    load_matrix lp constr;
    lp

external get_num_rows : lp -> int = "ocaml_glpk_get_num_rows"

external get_num_cols : lp -> int = "ocaml_glpk_get_num_cols"

let get_col_primals lp =
  let n = get_num_cols lp in
  let ans = Array.make n 0. in
    for i = 0 to (n - 1)
    do
      ans.(i) <- get_col_primal lp i
    done;
    ans

external scale_problem : lp -> unit = "ocaml_glpk_scale_problem"

external unscale_problem : lp -> unit = "ocaml_glpk_unscale_problem"

external interior : lp -> unit = "ocaml_glpk_interior"

external set_col_kind : lp -> int -> var_kind -> unit = "ocaml_glpk_set_col_kind"

external warm_up : lp -> unit = "ocaml_glpk_warm_up"

external branch_and_cut : lp -> unit = "ocaml_glpk_intopt"

external mip_status : lp -> status = "ocaml_glpk_mip_status"

external mip_obj_val : lp -> float = "ocaml_glpk_mip_obj_val"

external read_cplex : string -> lp = "ocaml_glpk_read_cplex"

external write_cplex : lp -> string -> unit = "ocaml_glpk_write_cplex"
