(**
  * OCaml bindings to glpk. Please see the glpk manual for further explanations
  * on the semantics of functions.
  *
  * Warning: contrarily to the C version of glpk, all indexes are 0-based.
  *
  * @author Samuel Mimram
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
  | Boolean_var (** boolean variable *)

(** Status of a solution. *)
type status =
  | Optimal (** optimal solution *)
  | Feasible (** feasible solution *)
  | Infeasible (** infeasible solution *)
  | No_feasible (** no feasible solution *)
  | Unbounded (** unbounded solution *)
  | Undefined (** undefined solution *)

(** {1 Exceptions} *)

(** Unknown error (this exception should disappear in future versions). *)
exception Unknown_error of int


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

(** Load a sparse constraint matrix stored as an array whose elements are of the
  * form ((row, column), value) indicating non-null elements of the matrix. *)
val load_sparse_matrix : lp -> ((int * int) * float) array -> unit


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

(** {2 Mixed integer programming} *)

(** Solve a MIP proble using the branch-and-cut method. *)
val branch_and_cut : lp -> unit

val mip_status : lp -> status

val mip_obj_val : lp -> float
