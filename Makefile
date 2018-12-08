scripts=$(wildcard test/*.py)

all:
	swig -c++ -python astro.i
	python2 setup.py build_ext --inplace  
	$(LIB)
	@rm -rf build

clean:
	@rm -rvf $(LIB) *.o */*.o *.so astro.py* astro_wrap.c* build

install:
	python2 setup.py install

test: $(scripts)

$(scripts): astro.py
	@python2 $@
