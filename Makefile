scripts=$(wildcard test/*.py)

install:
	swig -c++ -python hypatia.i
	python setup.py build_ext --inplace
	$(LIB)
	@rm -rf build
	sudo python setup.py install

clean:
	@rm -rvf $(LIB) *.o */*.o *.so hypatia.py* hypatia_wrap.c* build

test: $(scripts)

$(scripts): hypatia.py
	@python $@
