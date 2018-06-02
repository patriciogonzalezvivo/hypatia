#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
from __future__ import unicode_literals

import time
import datetime
import math

from astro import *
import ephem

LNG = -122.0442
LAT = 37.6393
TOLERANCE_DEG = 0.5

a_obs = Observer(LNG, LAT)
e_obs = ephem.Observer();
e_obs.lon = MathOps.toRadians(LNG)
e_obs.lat = MathOps.toRadians(LAT)
e_obs.elevation = 32

def test(a_body, e_body ):
  a_body.compute(a_obs)
  
  a_ra = a_body.getEquatorial().getRightAscension(DEGS)
  a_dec = a_body.getEquatorial().getDeclination(DEGS)
  a_alt = a_body.getHorizontal().getAltitud(DEGS)
  a_az = a_body.getHorizontal().getAzimuth(DEGS)

  e_ra = MathOps.toDegrees(float(repr(e_body.a_ra)))
  e_dec = MathOps.toDegrees(float(repr(e_body.a_dec)))
  e_alt = MathOps.toDegrees(float(repr(e_body.alt)))
  e_az = MathOps.toDegrees(float(repr(e_body.az)))

  d_ra = abs(a_ra - e_ra)
  d_dec = abs(a_dec - e_dec)
  d_alt = abs(a_alt - e_alt)
  d_az = abs(a_az - e_az)

  if (d_ra < TOLERANCE_DEG and d_dec < TOLERANCE_DEG and d_alt < TOLERANCE_DEG and d_az < TOLERANCE_DEG):
    return True
  else:

    print('---------------------------------------- ' + a_body.getBodyName())
    # print('Astro Ra: %s°      Dec: %s°' % (a_ra, a_dec))
    # print('Ephem Ra: %s°      Dec: %s°' % (e_ra, e_dec))
    print('DELTA Ra: %s°      Dec: %s°' % (d_ra, d_dec))
    # print('Astro Alt: %s°     Az: %s°' % (a_alt, a_az))
    # print('Ephem Alt: %s°     Az: %s°' % (e_alt, e_az))
    print('DELTA Alt: %s°     Az: %s°' % (d_alt, d_az))

  return False

tests = [ 
  test(Body(SUN), ephem.Sun(e_obs)),
  test(Body(MERCURY), ephem.Mercury(e_obs)),
  test(Body(VENUS), ephem.Venus(e_obs)),
  test(Body(LUNA), ephem.Moon(e_obs)),
  test(Body(MARS), ephem.Mars(e_obs)),
  test(Body(SATURN), ephem.Saturn(e_obs)),
  test(Body(JUPITER), ephem.Jupiter(e_obs)),
  test(Body(URANUS), ephem.Uranus(e_obs)),
  test(Body(NEPTUNE), ephem.Neptune(e_obs)),
  test(Body(PLUTO), ephem.Pluto(e_obs))
]

check = True
for i in range(0, len(tests)):
  if not tests[i]:
    check = False
    print("Test number",str(i), "fail")

if not check:
  print(__file__, "FAILURE")
else:
  print(__file__, "SUCESS")
