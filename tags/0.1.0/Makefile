# $Id$

all: byte

byte:
	make -C src byte

opt:
	make -C src opt

clean:
	make -C src clean

distclean: clean
	rm -rf doc

doc: byte
	mkdir -p doc/html
	ocamldoc -html -m A -keep-code -I src -d doc/html src/glpk.mli src/glpk.ml

install:
	make -C src install

uninstall:
	make -C src uninstall

.PHONY: doc
