scripts=$(wildcard test/*.py)

install:
	swig -c++ -python hypatia.i
	python3 -m pip install -e .

clean:
	@rm -rvf $(LIB) *.o */*.o *.so hypatia.py* hypatia_wrap.c* build

test: $(scripts)

$(scripts): hypatia.py
	@python $@
