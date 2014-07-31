SOURCES = oldapi/lpx.c glpk_stubs.c glpk.mli glpk.ml
RESULT = glpk
LIBINSTALL_FILES = $(wildcard *.mli *.cmi *.cma *.cmxa *.a *.so)
CLIBS = glpk
CFLAGS = -Wall -ansi -DCAML_NAME_SPACE -g
OCAMLDOCFLAGS = -stars

ANNOTATE = yes

all: byte

byte: byte-code-library

opt: native-code-library

install: libinstall

uninstall: libuninstall

update: uninstall install

-include OCamlMakefile
