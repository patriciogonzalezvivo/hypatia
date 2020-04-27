#!/usr/bin/env python

"""
hypatia: is an astronomy library based on Mark Huss's and Bill Gray's code. Both authors have very technical and ambitious projects, with over comboluted code for my porposes. I mostly:
- simplify the C++ interfaces by making simpler and clear objects and classes
- clean the dependences so the C++ code can be added to other C++ projects easily
- add Python package which makes a C++ wrapper usign swig
"""

from distutils.core import setup, Extension

doc_lines = __doc__.split('\n')
hypatia_module = Extension(  
  '_hypatia',
  include_dirs=['include'],
  sources= [ 
    'hypatia_wrap.cxx', 
    'src/primitives/Polar.cpp',
    'src/primitives/Vector2.cpp',
    'src/primitives/Vector3.cpp',
    'src/primitives/TimeSpan.cpp',
    'src/primitives/DateTime.cpp',
    'src/coordinates/Galactic.cpp',
    'src/coordinates/Ecliptic.cpp',
    'src/coordinates/Equatorial.cpp',
    'src/coordinates/Horizontal.cpp',
    'src/coordinates/Geodetic.cpp',
    'src/coordinates/ECI.cpp',
    'src/coordinates/Tile.cpp',
    'src/MathOps.cpp',
    'src/TimeOps.cpp',
    'src/CoordOps.cpp',
    'src/ProjOps.cpp',
    'src/Observer.cpp', 
    'src/Body.cpp', 
    'src/Luna.cpp', 
    'src/Star.cpp',
    'src/Constellation.cpp',
    'src/Satellite.cpp',
    'src/models/VSOP87.cpp',
    'src/models/Pluto.cpp',
    'src/models/TLE.cpp',
    'src/models/Orbit.cpp',
    'src/models/SGP4.cpp'
   ],
   swig_opts = ['-c++']
)

setup(  
  name = 'hypatia',
  description = doc_lines[0],
  long_description = '\n'.join(doc_lines[2:]),
  version     = '0.2',
  author      = 'Patricio Gonzalez Vivo',
  author_email = 'patriciogonzalezvivo@gmail.com',
  license     = "BSD",
  ext_modules = [ hypatia_module ],
  py_modules = [ 'hypatia' ],
)
