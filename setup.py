#!/usr/bin/env python

"""
astro:

"""

from distutils.core import setup, Extension

doc_lines = __doc__.split('\n')
astro_module = Extension(  '_astro',
                           sources=[  'astro_wrap.cxx', 
                                      'src/MathOps.cpp', 
                                      'src/TimeOps.cpp', 
                                      'src/ObsInf.cpp', 
                                      'src/AstroOps.cpp', 
                                      'src/Vsop.cpp', 
                                      'src/PlanetData.cpp', 
                                      'src/Lunar.cpp', 
                                      'src/Pluto.cpp'
                                    ],
                           swig_opts = ['-c++']
                           )

setup(  name = 'astro',
        description = doc_lines[0],
        long_description = '\n'.join(doc_lines[2:]),
        version     = '0.1',
        author      = 'Patricio Gonzalez Vivo',
        author_email = 'patriciogonzalezvivo@gmail.com',
        ext_modules = [ astro_module ],
        py_modules = [ 'astro' ],
    )
