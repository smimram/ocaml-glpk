open Glpk

let () =
  (*
    let lp = new_problem () in
    set_prob_name lp "sample";
    set_direction lp Maximize;
    add_rows lp 3;
    set_row_name lp 1 "p";
    set_row_bounds lp 1 Upper_bounded_var 0. 100.;
    set_row_name lp 2 "q";
    set_row_bounds lp 2 Upper_bounded_var 0. 600.;
    set_row_name lp 3 "r";
    set_row_bounds lp 3 Upper_bounded_var 0. 300.;
    add_columns lp 3;
    set_col_name lp 1 "x1";
    set_col_bounds lp 1 Lower_bounded_var 0. 0.;
    set_obj_coef lp 1 10.;
    set_col_name lp 2 "x2";
    set_col_bounds lp 2 Lower_bounded_var 0. 0.;
    set_obj_coef lp 2 6.;
    set_col_name lp 3 "x3";
    set_col_bounds lp 3 Lower_bounded_var 0. 0.;
    set_obj_coef lp 3 4.;
    load_matrix lp
    [|
    [|1.; 1.; 1.|];
    [|10.; 4.; 5.|];
    [|2.; 2.; 6.|]
    |]; *)
  let lp = make_problem Maximize
	     [|10.; 6.; 4.|]
	     [|
	       [|1.; 1.; 1.|];
	       [|10.; 4.; 5.|];
	       [|2.; 2.; 6.|]
	     |]
	     [| -.infinity, 100.; -.infinity, 600.; -.infinity, 300. |]
	     [| 0., infinity; 0., infinity; 0., infinity|] in
    scale_problem lp;
    simplex lp;
    let prim = get_col_primals lp in
      Printf.printf "Z: %g    x0: %g    x1: %g    x2: %g\n%!" (get_obj_val lp) prim.(0) prim.(1) prim.(2)
