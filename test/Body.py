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
  print( "[ " + a_body.getBodyName() + " ]")
  ok = True

  # a_hlat = a_body.getEclipticHeliocentric().getLatitude(DEGS)
  # a_hlng = a_body.getEclipticHeliocentric().getLongitude(DEGS)
  # e_hlat = MathOps.toDegrees(float(repr(e_body.hlat)))
  # e_hlng = MathOps.toDegrees(float(repr(e_body.hlon)))
  # d_hlat = abs(a_hlat - e_hlat)
  # d_hlng = abs(a_hlng - e_hlng)
  # if (d_hlat > TOLERANCE_DEG or d_hlng > TOLERANCE_DEG):
  #   # print('Astro Lat: %s°      Lng: %s°' % (a_hlat, a_hlng))
  #   # print('Ephem Lat: %s°      Lng: %s°' % (e_hlat, e_hlng))
  #   print('DELTA Lat: %s°      Lng: %s°' % (d_hlat, d_hlng))
  #   ok = False

  a_ra = a_body.getEquatorial().getRightAscension(DEGS)
  a_dec = a_body.getEquatorial().getDeclination(DEGS)
  e_ra = MathOps.toDegrees(float(repr(e_body.g_ra)))
  e_dec = MathOps.toDegrees(float(repr(e_body.g_dec)))
  d_ra = abs(a_ra - e_ra)
  d_dec = abs(a_dec - e_dec)
  if (d_ra > TOLERANCE_DEG or d_dec > TOLERANCE_DEG):
    # print('Astro Ra: %s°      Dec: %s°' % (a_ra, a_dec))
    # print('Ephem Ra: %s°      Dec: %s°' % (e_ra, e_dec))
    print('DELTA Ra: %s°      Dec: %s°' % (d_ra, d_dec))
    ok = False

  a_alt = a_body.getHorizontal().getAltitud(DEGS)
  a_az = a_body.getHorizontal().getAzimuth(DEGS)
  e_alt = MathOps.toDegrees(float(repr(e_body.alt)))
  e_az = MathOps.toDegrees(float(repr(e_body.az)))
  d_alt = abs(a_alt - e_alt)
  d_az = abs(a_az - e_az)

  if (d_alt > TOLERANCE_DEG or d_az > TOLERANCE_DEG):
    # print('Astro Alt: %s°     Az: %s°' % (a_alt, a_az))
    # print('Ephem Alt: %s°     Az: %s°' % (e_alt, e_az)) 
    print('DELTA Alt: %s°     Az: %s°' % (d_alt, d_az))
    ok = False
    
  return ok

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
