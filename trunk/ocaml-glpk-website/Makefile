all: doc update

update:
	scp -C -r htdocs/* shell.sourceforge.net:/home/groups/o/oc/ocaml-glpk/htdocs

doc:
	make -C ../ocaml-glpk doc
	cp ../ocaml-glpk/doc/html/* htdocs/ocamldoc
