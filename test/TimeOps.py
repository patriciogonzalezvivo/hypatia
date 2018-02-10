#!/usr/bin/env python

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
from __future__ import unicode_literals

import time
import datetime
import math

from astro import *
import ephem

def testJDtoDatetimetoJD(jd):
  ymd = TimeOps.toYMD(jd)
  hms = TimeOps.toHMS(jd)
  expected = TimeOps.toJD(int(ymd[0]),int(ymd[1]),int(math.floor(ymd[2])), int(hms[0]), int(hms[1]), int(hms[2]))
  check = jd == expected
  if not check:
    print( "[FAIL]", str("{:.1f}".format(jd)), "is not",  expected )

  return check

def testJDtoDatetime(jd, expected):
  dt = TimeOps.formatDateTime(jd, Y_M_D_HM)
  check = dt[0:len(dt)-2] == expected[0:len(expected)-2]
  if not check:
    print( "[FAIL]", str("{:.1f}".format(jd)), "(", dt, ") is not",  expected )

  return check

def testJDtoYMD(jd, expected):
  ymd = TimeOps.toYMD(jd)
  hms = TimeOps.toHMS(jd)
  dt = str(ymd[0])+'-'+"{:0>2d}".format(ymd[1])+'-'+"{:.1f}".format(ymd[2])
  dt = str(dt)

  check = dt == expected
  if not check:
    print( "[FAIL]",str("{:.1f}".format(jd)), "(", dt, ") is not",  expected )

  return check

def testDOW(jd, expected):
  check = TimeOps.toDOW(jd) == expected
  if not check:
    dt = TimeOps.formatDateTime(jd, Y_M_D_HM)
    print( "[FAIL]",str("{:.1f}".format(jd)), "(", dt, ") is not",  expected )

  return check

now_jd = TimeOps.now()
now_sec = TimeOps.getCurrentSeconds();
now_date = datetime.datetime.utcnow()
utc_date = now_date.strftime('%Y-%m-%d %H:%M')

tests = [ 
  testJDtoDatetime(TimeOps.toJD(now_sec), utc_date ),
  testJDtoDatetime(ephem.julian_date(now_date), utc_date ),
  testJDtoDatetime(now_jd, utc_date ),
  testJDtoYMD(2451545.0, '2000-01-1.5'), 
  testJDtoYMD(2451179.5, '1999-01-1.0'),
  testJDtoYMD(2446822.5, '1987-01-27.0'),
  testJDtoYMD(2446966.0, '1987-06-19.5'),
  testJDtoYMD(2415020.5, '1900-01-1.0'),
  testJDtoYMD(2305447.5, '1600-01-1.0'),
  testJDtoYMD(2305812.5, '1600-12-31.0'),
  testJDtoYMD(2026871.8, '837-04-10.3'),
  testJDtoYMD(1676496.5, '-123-12-31.0'),
  testJDtoYMD(1676497.5, '-122-01-1.0'),
  testJDtoYMD(1356001.0, '-1000-07-12.5'),
  testJDtoYMD(1355866.5, '-1000-02-29.0'),
  testJDtoYMD(1355671.4, '-1001-08-17.9'),
  testJDtoYMD(0.0, '-4712-01-1.5'),
  testDOW(2434923.5, 3),
  testJDtoDatetimetoJD(2451545.0),
  testJDtoDatetimetoJD(2451179.5),
  testJDtoDatetimetoJD(2446822.5),
  testJDtoDatetimetoJD(2446966.0),
  testJDtoDatetimetoJD(2415020.5),
  testJDtoDatetimetoJD(2305447.5),
  testJDtoDatetimetoJD(2305812.5),
  testJDtoDatetimetoJD(2026871.8),
  testJDtoDatetimetoJD(1676496.5),
  testJDtoDatetimetoJD(1676497.5),
  testJDtoDatetimetoJD(1356001.0),
  testJDtoDatetimetoJD(1355866.5),
  testJDtoDatetimetoJD(1355671.4),
]

print(TimeOps.toYMD(2026871.8))
print(TimeOps.toHMS(2026871.8))
print(TimeOps.toJD(1999,01,1))
print(MathOps.toDegrees(12, 0, 0) / 24.0)

check = True
for i in range(0, len(tests)):
  if not tests[i]:
    check = False
    print("Test number",str(i), "fail")

if not check:
  print(__file__, "FAILURE")
else:
  print(__file__, "SUCESS")
