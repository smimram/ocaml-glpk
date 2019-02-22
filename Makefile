
NAME=glpk

build:
	@dune build @install -p $(NAME)

clean:
	@dune clean

doc:
	@dune build @doc

example: build
	@dune exec examples/example.exe

install: build
	@dune install

uninstall:
	@dune uninstall
