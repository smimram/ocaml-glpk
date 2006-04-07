RCP = rsync -rLtCPz --delete --stats --exclude "access.log"

all: doc update

update:
	$(RCP) htdocs shell.sourceforge.net:/home/groups/o/oc/ocaml-glpk

doc:
	make -C ../trunk/ocaml-glpk doc
	cp ../trunk/ocaml-glpk/doc/html/* htdocs/ocamldoc
	cp ../trunk/ocaml-glpk/examples/example.ml htdocs/
