RCP = rsync -rLtCP --delete --stats

all: doc update

update:
	$(RCP) htdocs shell.sourceforge.net:/home/groups/o/oc/ocaml-glpk

doc:
	make -C ../ocaml-glpk doc
	cp ../ocaml-glpk/doc/html/* htdocs/ocamldoc
	cp ../ocaml-glpk/examples/example.ml htdocs/
