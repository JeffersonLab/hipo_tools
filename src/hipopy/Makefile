.PHONY: clean
all: setup tests

setup:
	python setup.py build_ext --inplace

tests:
	python test.py

clean:
	-rm -f hipopy.cpp hipopy.cpython-36m-darwin.so
