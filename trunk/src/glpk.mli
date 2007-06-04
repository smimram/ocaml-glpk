(**
  * OCaml bindings to glpk. Please see the glpk manual for further explanations 
  * on the semantics of functions.
  *
  * Warning: contrarily to the C version of glpk, all indexes are 0-based.
  *
  * @author Samuel Mimram
  *)


(* $Id$ *)


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

(** The problem has no rows and/or column. *)
exception Empty

(** The LP basis is invalid beacause the number of basic variables is not the same as the number of rows. *)
exception Bad_basis

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

(** Very slow convergence or divergence. *)
exception No_convergence

(** Failure of the solver (the current basis matrix got singular or ill-conditionned). *)
exception Solver_failure

(** Unknown error (this exception should disappear in future versions). *)
exception Unknown_error


(** {1 Functions} *)

(** {2 Creating, reading and saving problems} *)

(** Create a new linear programmation problem. *)
val new_problem : unit -> lp

(** [make_problem dir zcoefs constrs pbounds xbounds] creates the new linear programmation problem where Z = Sum_i [zcoefs.(i)] * x_ i should be optimized in the direction [dir] under the constraints [fst pbounds.(i)] <= p_i <= [snd pbounds.(i)] and [fst xbounds.(i)] <= x_i <= [snd xbounds.(i)] where p_i = Sum_j [constrs.(i).(j)] * x_j. The bounds may be [+] / [- infinity]. *)
val make_problem : direction -> float array -> float array array -> (float * float) array -> (float * float) array -> lp

(** Read problem data in CPLEX LP format from a file. *)
val read_cplex : string -> lp

(** Write prblem data in CPLEX LP format into a file. *)
val write_cplex : lp -> string -> unit


(** {2 Setting and retreiving paramters of a problem} *)

(** Set the problem name. *)
val set_prob_name : lp -> string -> unit

(** Retrieve the problem name. *)
val get_prob_name : lp -> string

(** Set the problem class. *)
val set_class : lp -> prob_class -> unit

(** Retrieve the problem class. *)
val get_class : lp -> prob_class

(** Set the direction of the optimization. *)
val set_direction : lp -> direction -> unit

(** Retrieve the direction of the optimization. *)
val get_direction : lp -> direction

(** Set the objective name. *)
val set_obj_name : lp -> string -> unit

(** Retrieve the objective name. *)
val get_obj_name : lp -> string

(** Add rows. *)
val add_rows : lp -> int -> unit

(** Retreive the number of rows. *)
val get_num_rows : lp -> int

(** Set the name of a row. *)
val set_row_name : lp -> int -> string -> unit

(** Retrieve the name of a row. *)
val get_row_name : lp -> int -> string

(** Set a row bound. *)
val set_row_bounds : lp -> int -> aux_var_type -> float -> float -> unit

(** Add columns. *)
val add_columns : lp -> int -> unit

(** Retreive the number of columns. *)
val get_num_cols : lp -> int

(** Set the name of a column. *)
val set_col_name : lp -> int -> string -> unit

(** Retrieve the name of a column. *)
val get_col_name : lp -> int -> string

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

(** Warm up the LP basis for the specified problem object using current statuses assigned to rows and columns. *)
val warm_up : lp -> unit

(** Solve an LP problem using the simplex method. You must use builtin presolver
  * (see [use_presolver]) to get an exception if the problem has no feasible
  * solution. *)
val simplex : lp -> unit

(** Solve an LP problem using the primal-dual interior point method. *)
val interior : lp -> unit

(** Solve a MIP proble using the branch-and-bound method. *)
val branch_and_bound : lp -> unit

(** Solve a MIP proble using and optimized version of the branch-and-bound method. *)
val branch_and_bound_opt : lp -> unit

(** Retrieve objective value. *)
val get_obj_val : lp -> float

(** Get the primal value of the structural variable associated with a column. *)
val get_col_primal : lp -> int -> float

(** Get the primal values of the structural variables associated with each column. *)
val get_col_primals : lp -> float array

(** Get the primal value of the structural variable associated with a row. *)
val get_row_primal : lp -> int -> float

(** Get the dual value of the structural variable associated with a row. *)
val get_row_dual : lp -> int -> float


(** {2 Setting parameters of the solver} *)

(** Set the level of messages output by sover routines. The second argument might be:
  - 0: no output
  - 1: error message only
  - 2: normal output
  - 3: full output (includes informational messages)
*)
val set_message_level : lp -> int -> unit

(** Use the builtin LP-presolver in [simplex]? *)
val use_presolver : lp -> bool -> unit

(** Initialize the simplex iteration counter. *)
val set_simplex_iteration_count : lp -> int -> unit

(** Reset the simplex iteration counter. *)
val reset_simplex_iteration_count : lp -> unit

(** This number is incremented after each simplex iteration. *)
val get_simplex_iteration_count : lp -> int

(** Set the maximum number of iterations that [simplex] should do. *)
val set_simplex_iteration_limit : lp -> int -> unit

(** Retrieve the maximum number of iterations that [simplex] should do. *)
val get_simplex_iteration_limit : lp -> int

(** Set the maximum amount of time that [simplex] should take. *)
val set_simplex_time_limit : lp -> float -> unit

(** Retrieve the maximum amount of time that [simplex] should take. *)
val get_simplex_time_limit : lp -> float
