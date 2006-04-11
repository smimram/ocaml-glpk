(*
 * ocaml-glpk - OCaml bindings to glpk
 * Copyright (C) 2004-2006 Samuel Mimram
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

(* $Id$ *)

type lp

type direction = Minimize | Maximize

type aux_var_type = Free_var | Lower_bounded_var | Upper_bounded_var | Double_bounded_var | Fixed_var

type prob_class = Linear_prog | Mixed_integer_prog

type var_kind = Continuous_var | Integer_var

exception Fault
exception Lower_limit
exception Upper_limit
exception No_primal_feasible_solution
exception No_dual_feasible_solution
exception Iteration_limit
exception Time_limit
exception Solver_failure
exception Empty
exception Bad_basis
exception No_convergence
exception Unknown_error

let _ =
  Callback.register_exception "ocaml_glpk_exn_fault" Fault;
  Callback.register_exception "ocaml_glpk_exn_objll" Lower_limit;
  Callback.register_exception "ocaml_glpk_exn_objul" Upper_limit;
  Callback.register_exception "ocaml_glpk_exn_nopfs" No_primal_feasible_solution;
  Callback.register_exception "ocaml_glpk_exn_nodfs" No_dual_feasible_solution;
  Callback.register_exception "ocaml_glpk_exn_itlim" Iteration_limit;
  Callback.register_exception "ocaml_glpk_exn_tmlim" Time_limit;
  Callback.register_exception "ocaml_glpk_exn_sing" Solver_failure;
  Callback.register_exception "ocaml_glpk_exn_empty" Empty;
  Callback.register_exception "ocaml_glpk_exn_badb" Bad_basis;
  Callback.register_exception "ocaml_glpk_exn_noconv" No_convergence;
  Callback.register_exception "ocaml_glpk_exn_unkown" Unknown_error;

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

external simplex : lp -> unit = "ocaml_glpk_simplex"

external get_obj_val : lp -> float = "ocaml_glpk_get_obj_val"

external get_col_primal : lp -> int -> float = "ocaml_glpk_get_col_prim"

let make_problem dir zcoefs constr pbounds xbounds =
  let lp = new_problem () in
    set_direction lp dir;
    add_rows lp (Array.length pbounds);
    for i = 0 to ((Array.length pbounds) - 1)
    do
      match pbounds.(i) with
        | lb, ub when lb = -.infinity && ub = infinity -> set_row_bounds lp i Free_var 0. 0.
        | lb, ub when ub = infinity -> set_row_bounds lp i Lower_bounded_var lb 0.
        | lb, ub when lb = -.infinity -> set_row_bounds lp i Upper_bounded_var 0. ub
        | lb, ub when lb = ub -> set_row_bounds lp i Fixed_var lb ub
        | lb, ub -> set_row_bounds lp i Double_bounded_var lb ub
    done;
    add_columns lp (Array.length xbounds);
    for i = 0 to ((Array.length xbounds) - 1)
    do
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

external set_class : lp -> prob_class -> unit = "ocaml_glpk_set_class"

external get_class : lp -> prob_class = "ocaml_glpk_get_class"

external set_col_kind : lp -> int -> var_kind -> unit = "ocaml_glpk_set_col_kind"

external branch_and_bound : lp -> unit = "ocaml_glpk_integer"

external warm_up : lp -> unit = "ocaml_glpk_warm_up"

external use_presolver : lp -> bool -> unit = "ocaml_glpk_use_presolver"

external read_cplex : string -> lp = "ocaml_glpk_read_cplex"

external write_cplex : lp -> string -> unit = "ocaml_glpk_write_cplex"

external set_simplex_iteration_count : lp -> int -> unit = "ocaml_glpk_set_iteration_count"

let reset_simplex_iteration_count lp =
  set_simplex_iteration_count lp 0

external get_simplex_iteration_count : lp -> int = "ocaml_glpk_get_iteration_count"

external _set_message_level : lp -> int -> unit = "ocaml_glpk_set_message_level"

let set_message_level lp n =
    if (n < 0 && n > 3) then
        raise (Invalid_argument "set_message_level");
    _set_message_level lp n
