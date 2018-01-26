#!/usr/bin/env python

"""
astro: is an astronomy library based on Mark Huss's and Bill Gray's code. Both authors have very technical and ambitious projects, with over comboluted code for my porposes. I mostly:
- simplify the C++ interfaces by making simpler and clear objects and classes
- clean the dependences so the C++ code can be added to other C++ projects easily
- add Python package which makes a C++ wrapper usign swig
"""

from distutils.core import setup, Extension

doc_lines = __doc__.split('\n')
astro_module = Extension(  
  '_astro',
  sources= [ 
    'astro_wrap.cxx', 
    'src/MathOps.cpp', 
    'src/TimeOps.cpp', 
    'src/Vector.cpp',
    'src/Observer.cpp', 
    'src/AstroOps.cpp',
    'src/EqPoint.cpp',
    'src/ProjOps.cpp',
    'src/Vsop.cpp', 
    'src/Body.cpp', 
    'src/Luna.cpp', 
    'src/Pluto.cpp',
    'src/Constellation.cpp',
    'src/Star.cpp'
   ],
   swig_opts = ['-c++']
)

setup(  
  name = 'astro',
  description = doc_lines[0],
  long_description = '\n'.join(doc_lines[2:]),
  version     = '0.1',
  author      = 'Patricio Gonzalez Vivo',
  author_email = 'patriciogonzalezvivo@gmail.com',
  ext_modules = [ astro_module ],
  py_modules = [ 'astro' ],
)
