.PHONY: clean
all: setup

setup:
	python setup.py build_ext --inplace
	python test.py

clean:
	-rm -f hipopy.cpp hipopy.cpython-36m-darwin.so
