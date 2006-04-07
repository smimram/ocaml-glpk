open Glpk

let make_ukp z c b =
  let n = Array.length z in
  let bbounds = Array.make 1 (0., b) in
  let xbounds = Array.make n (0., infinity) in
  let constrs = Array.init 1 (fun _ -> Array.init n (fun j -> c.(j))) in
  let lp = make_problem Maximize z constrs bbounds xbounds in
    set_class lp Mixed_integer_prog;
    Array.iteri (fun i _ -> set_col_kind lp i Integer_var) c;
    scale_problem lp;
    use_presolver lp true;
    lp

let () =
  let lp = make_ukp [|3.; 5.; 7.|] [|12.; 3.; 9.|] 100. in
    simplex lp;
    branch_and_bound lp;
    let prim = get_col_primals lp in
      Printf.printf "Z: %g    x0: %g    x1: %g    x2: %g\n%!" (get_obj_val lp) prim.(0) prim.(1) prim.(2)
