(*
  ocaml-glpk - OCaml bindings to glpk
  Copyright (C) 2004 Samuel Mimram
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330,
  Boston, MA 02111-1307, USA.
*)

(* $Id$ *)

type lp

type direction = Minimize | Maximize

type aux_var_type = Free_var | Lower_bounded_var | Upper_bounded_var | Double_bounded_var | Fixed_var

type prob_class = Linear_prog | Mixed_integer_prog

type var_kind = Continuous_var | Integer_var

external new_problem : unit -> lp = "ocaml_glpk_new_prob"

external set_prob_name : lp -> string -> unit = "ocaml_glpk_set_prob_name"

external set_obj_name : lp -> string -> unit = "ocaml_glpk_set_obj_name"

external set_direction : lp -> direction -> unit = "ocaml_glpk_set_direction"

external add_rows : lp -> int -> unit = "ocaml_glpk_add_rows"

external set_row_name : lp -> int -> string -> unit = "ocaml_glpk_set_row_name"

external set_row_bounds : lp -> int -> aux_var_type -> float -> float -> unit = "ocaml_glpk_set_row_bounds"

external add_columns : lp -> int -> unit = "ocaml_glpk_add_cols"

external set_col_name : lp -> int -> string -> unit = "ocaml_glpk_set_col_name"

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

let get_col_primals lp =
  let n = get_num_rows lp in
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

external set_col_kind : lp -> int -> var_kind -> unit = "ocaml_glpk_set_col_kind"

external branch_and_bound : lp -> unit = "ocaml_glpk_integer"

(* TODO: abstract type argument? *)
external set_message_level : lp -> int -> unit = "ocaml_glpk_set_message_level"
