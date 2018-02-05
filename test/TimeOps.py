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

def testJDtoDatetime(jd, expeceted):
  datetime = TimeOps.formatDateTime(jd, Y_M_D_HM)
  return str("{:.1f}".format(jd)), datetime, ("[OK]" if datetime[0:len(datetime)-2] == expeceted[0:len(expeceted)-2] else  "[FAIL]")

def testJDtoYMD(jd, expeceted):
  ymd = TimeOps.toYMD(jd)
  hms = TimeOps.toHMS(jd)
  datetime = str(ymd[0])+'-'+"{:0>2d}".format(ymd[1])+'-'+"{:.1f}".format(ymd[2])
  datetime = str(datetime)
  return str("{:.1f}".format(jd)), datetime, ("[OK]" if datetime == expeceted else  "[FAIL]")

now_jd = TimeOps.now()
now_sec = TimeOps.getCurrentSeconds();
now_date = datetime.datetime.utcnow()
utc_date = now_date.strftime('%Y-%m-%d %H:%M')

print('------------------------------------------------------------')
print('Julian Date (UTC sec):\t', testJDtoDatetime(TimeOps.toJD(now_sec) + TimeOps.JD_DIFF, utc_date ) )
print('Julian Date (ephem):\t\t', testJDtoDatetime(ephem.julian_date(now_date) + TimeOps.JD_DIFF, utc_date ) )
print('Julian Date (UTC now()):', testJDtoDatetime(now_jd, utc_date ) )

print('------------------------------------------------------------')

print(testJDtoYMD(2451545.0, '2000-01-1.5')) 
print(testJDtoYMD(2451179.5, '1999-01-1.0'))
print(testJDtoYMD(2446822.5, '1987-01-27.0'))
print(testJDtoYMD(2446966.0, '1987-06-19.5'))
print(testJDtoYMD(2415020.5, '1900-01-1.0'))
print(testJDtoYMD(2305447.5, '1600-01-1.0'))
print(testJDtoYMD(2305812.5, '1600-12-31.0'))
print(testJDtoYMD(2026871.8, '837-04-10.3'))
print(testJDtoYMD(1676496.5, '-123-12-31.0'))
print(testJDtoYMD(1676497.5, '-122-01-1.0'))
print(testJDtoYMD(1356001.0, '-1000-07-12.5'))
print(testJDtoYMD(1355866.5, '-1000-02-29.0'))
print(testJDtoYMD(1355671.4, '-1001-08-17.9'))
print(testJDtoYMD(0.0, '-4712-01-1.5'))


