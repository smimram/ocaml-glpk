# $Id$

SOURCES = glpk_stubs.c glpk.mli glpk.ml
RESULT = glpk
LIBINSTALL_FILES = $(wildcard *.mli *.cmi *.cma *.cmxa *.a *.so)
CLIBS = glpk

ANNOTATE=yes

all: byte

byte: byte-code-library

opt: native-code-library

install: libinstall

uninstall: libuninstall

update: uninstall install

# TODO: remove
test: byte
	rm -f test.cmo
	ocamlc dllglpk_stubs.so glpk.cmo test.ml -o test

runtest: test
	LD_LIBRARY_PATH=. ./test

-include OCamlMakefile
