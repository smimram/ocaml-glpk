(**
  OCaml bindings to glpk.

  Warning: contrarily to the C version of glpk, all indexes are 0-based.

  @author Samuel Mimram
*)

(** {1 Types} *) (* TODO: better comment! *)

(** A linear programmation problem. *)
type lp

(** Direction of the optimization. *)
type direction = Minimize | Maximize

(** Type of bounds of an auxiliary variable. *)
type aux_var_type = Free_var | Lower_bounded_var | Upper_bounded_var | Double_bounded_var | Fixed_var

(** Class of a problem. *)
type prob_class =
  | Linear_prog (** linear programming *)
  | Mixed_integer_prog (** mixed integer programming *)

(** Kind of a variable. *)
type var_kind =
  | Continuous_var (** continuous variable *)
  | Integer_var (** integer variable *)

(** {1 Exceptions} *)

(** The problem has no rows/columns, or the initial basis is invalid, or the initial basis matrix is singular or ill-conditionned. *)
exception Fault

(** The objective function being minimized has reached its lower limit and continues decreasing. *)
exception Lower_limit

(** The objective function being maximized has reached its upper limit and continues increasing. *)
exception Upper_limit

(** The problem has no primal feasible solution. *)
exception No_primal_feasible_solution

(** The problem has no dual feasible solution. *)
exception No_dual_feasible_solution

(** Iterations limit exceeded. *)
exception Iteration_limit

(** Time limit exceeded. *)
exception Time_limit

(** Failure of the solver (the current basis matrix got singular or ill-conditionned). *)
exception Solver_failure

(** {1 Functions} *)

(** {2 Creating, defining and retreiving parameters of problems} *)

(** Create a new linear programmation problem. *)
val new_problem : unit -> lp

(** [make_problem dir zcoefs constrs pbounds xbounds] creates the new linear programmation problem where Z = Sum_i [zcoefs.(i)] * x_ i should be optimized in the direction [dir] under the constraints [fst pbounds.(i)] <= p_i <= [snd pbounds.(i)] and [fst xbounds.(i)] <= x_i <= [snd xbounds.(i)] where p_i = Sum_j [constrs.(i).(j)] * x_j. The bounds may be [+] / [- infinity]. *)
val make_problem : direction -> float array -> float array array -> (float * float) array -> (float * float) array -> lp

(** Set the problem name. *)
val set_prob_name : lp -> string -> unit

(** Retrieve the problem name. *)
val get_prob_name : lp -> string

(** Set the problem class. *)
val set_class : lp -> prob_class -> unit

(** Set the direction of the optimization. *)
val set_direction : lp -> direction -> unit

(** Set the objective name. *)
val set_obj_name : lp -> string -> unit

(** Retrieve the objective name. *)
val get_obj_name : lp -> string

(** Add rows. *)
val add_rows : lp -> int -> unit

(** Retreive the number of rows. *)
val get_num_rows : lp -> int

(** Set a row name. *)
val set_row_name : lp -> int -> string -> unit

(** Set a row bound. *)
val set_row_bounds : lp -> int -> aux_var_type -> float -> float -> unit

(** Add columns. *)
val add_columns : lp -> int -> unit

(** Set a column name. *)
val set_col_name : lp -> int -> string -> unit

(** Set column kind. *)
val set_col_kind : lp -> int -> var_kind -> unit

(** Set a column boudaries. *)
val set_col_bounds : lp -> int -> aux_var_type -> float -> float -> unit

(** Set an objective coefficient. *)
val set_obj_coef : lp -> int -> float -> unit

(** Load a constraint matrix. *)
val load_matrix : lp -> float array array -> unit

(** {2 Solving problems and retreiving solutions} *)

(** Scale problem data. *)
val scale_problem : lp -> unit

(** Unscale problem data. *)
val unscale_problem : lp -> unit

(** Solve an LP problem using the simplex method. *)
val simplex : lp -> unit

(** Solve an LP problem using the primal-dual interior point method. *)
val interior : lp -> unit

(** Solve a MIP proble using the branch-and-bound method. *)
val branch_and_bound : lp -> unit

(** Retrieve objective value. *)
val get_obj_val : lp -> float

(** Retrieve a row primal value. *)
val get_col_primal : lp -> int -> float

(** Get the primal values of the whole column. *)
val get_col_primals : lp -> float array

(** {2 Setting parameters of the solver} *)

(** Set the level of messages output by sover routines. The second argument might be:
  - 0: no output
  - 1: error message only
  - 2: normal output
  - 3: full output (includes informational messages)
*)
val set_message_level : lp -> int -> unit

(** {1 Loading / saving problems} *)
