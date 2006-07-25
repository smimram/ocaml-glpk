VERSION := $(shell cat ../trunk/VERSION)

debsrc:
	make -C ../trunk dist
	cp ../ocaml-glpk-$(VERSION).tar.gz ocaml-glpk_$(VERSION).orig.tar.gz
	rm -rf ocaml-glpk-$(VERSION)
	tar zxvf ocaml-glpk_$(VERSION).orig.tar.gz
	cp -r ../trunk/debian ocaml-glpk-$(VERSION)
	cd ocaml-glpk-$(VERSION)/debian; rm -rf $$(find . -name .svn)

deb: debsrc
	cd ocaml-glpk-$(VERSION); debuild

clean:
	rm -f *.deb *.diff.gz *.dsc *.asc *.build *.changes *.orig.tar.gz

.PHONY: deb
