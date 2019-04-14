scripts=$(wildcard test/*.py)

install:
	swig -c++ -python astro.i
	python2 setup.py build_ext --inplace
	$(LIB)
	@rm -rf build
	python2 setup.py install


install_3:
	swig -c++ -python astro.i
	python3 setup.py build_ext --inplace
	$(LIB)
	@rm -rf build
	python3 setup.py install

install_anaconda3:
	# swig -c++ -python astro.i
	# /anaconda3/bin/./python3 setup.py build_ext --inplace
	# $(LIB)
	# @rm -rf build
	/anaconda3/bin/./python3.7 setup.py install

clean:
	@rm -rvf $(LIB) *.o */*.o *.so astro.py* astro_wrap.c* build

test: $(scripts)

$(scripts): astro.py
	@python $@
