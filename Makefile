scripts=$(wildcard test/*.py)

all:
	swig -c++ -python astro.i
	python setup.py build_ext --inplace  
	$(LIB)
	@rm -rf build

clean:
	@rm -rvf $(LIB) *.o */*.o *.so astro.py* astro_wrap.c* build

install:
	python setup.py install

test: $(scripts)

$(scripts): astro.py
	@python $@
