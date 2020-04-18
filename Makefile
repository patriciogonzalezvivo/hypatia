scripts=$(wildcard test/*.py)

install:
	swig -c++ -python hypatia.i
	python2 setup.py build_ext --inplace
	$(LIB)
	@rm -rf build
	sudo python2 setup.py install


install_3:
	swig -c++ -python hypatia.i
	# python3 setup.py build_ext --inplace
	# $(LIB)
	# @rm -rf build
	sudo python3 setup.py install

install_anaconda3:
	# swig -c++ -python hypatia.i
	# /anaconda3/bin/./python3 setup.py build_ext --inplace
	# $(LIB)
	# @rm -rf build
	/anaconda3/bin/./python3.7 setup.py install

clean:
	@rm -rvf $(LIB) *.o */*.o *.so hypatia.py* hypatia_wrap.c* build

test: $(scripts)

$(scripts): hypatia.py
	@python $@
