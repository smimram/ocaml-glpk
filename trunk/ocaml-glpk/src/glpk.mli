(**
  OCaml bindings to glpk.

  Warning: contrarily to the C version of glpk, all indexes are 0-based.

  @author Samuel Mimram
*)

(** A linear programmation problem. *)
type lp

(** Direction of the optimization. *)
type direction = Minimize | Maximize

(** Type of bounds of an auxiliary variable. *)
type aux_var_type = Free_var | Lower_bounded_var | Upper_bounded_var | Double_bounded_var | Fixed_var

(** Create a new linear programmation problem. *)
val new_problem : unit -> lp

(** [make_problem dir zcoefs constrs pbounds xbounds] creates the new linear problem where Z = Sum_i [zcoefs.(i)] * x_ i should be optimized in the direction [dir] under the constraints [fst pbounds.(i)] <= p_i <= [snd pbounds.(i)] and [fst xbounds.(i)] <= x_i <= [snd xbounds.(i)] where p_i = Sum_j [constrs.(i).(j)] * x_j. The bounds may be [+] / [- infinity]. *)
val make_problem : direction -> float array -> float array array -> (float * float) array -> (float * float) array -> lp

(** Set the problem's name. *)
val set_prob_name : lp -> string -> unit

(** Set the direction of the optimization. *)
val set_direction : lp -> direction -> unit

(** Set the objective name. *)
val set_obj_name : lp -> string -> unit

(** Add rows. *)
val add_rows : lp -> int -> unit

(** Set a row name. *)
val set_row_name : lp -> int -> string -> unit

(** Set a row bound. *)
val set_row_bounds : lp -> int -> aux_var_type -> float -> float -> unit

(** Add columns. *)
val add_columns : lp -> int -> unit

(** Set a column name. *)
val set_col_name : lp -> int -> string -> unit

(** Set a column boudaries. *)
val set_col_bounds : lp -> int -> aux_var_type -> float -> float -> unit

(** Set an objective coefficient. *)
val set_obj_coef : lp -> int -> float -> unit

(** Load a constraint matrix. *)
val load_matrix : lp -> float array array -> unit

(** Solve the LP problem using the simplex method. *)
val simplex : lp -> unit

(** Retrieve objective value. *)
val get_obj_val : lp -> float

(** Retrieve a row primal value. *)
val get_col_primal : lp -> int -> float

val get_num_rows : lp -> int

val get_primal : lp -> float array

val scale_problem : lp -> unit
