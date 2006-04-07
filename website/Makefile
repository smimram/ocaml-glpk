RCP = rsync -rLtCPz --delete --stats --exclude "access.log"

all: doc update

update:
	$(RCP) htdocs shell.sourceforge.net:/home/groups/o/oc/ocaml-glpk

doc:
	make -C ../trunk doc
	cp ../trunk/doc/html/* htdocs/ocamldoc
	cp ../trunk/examples/example.ml htdocs/
