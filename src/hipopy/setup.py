from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

setup(ext_modules=[Extension("pylibhipo",
                             ["pylibhipo.pyx"],
                             language="c++",
                             include_dirs=["hipo"],
                             extra_compile_args=[
                                 "-pthread", "-stdlib=libc++", "-std=c++1z", "-m64"],
                             extra_link_args=["-pthread", "-stdlib=libc++", "-std=c++1z", "-m64"],)],
      cmdclass={'build_ext': build_ext})
