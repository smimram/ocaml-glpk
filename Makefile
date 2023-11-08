
NAME=glpk

build:
	@dune build @install -p $(NAME)

clean:
	@dune clean

doc:
	@dune build @doc

examples: build
	@dune exec examples/example.exe

install: build
	@dune install

uninstall:
	@dune uninstall
