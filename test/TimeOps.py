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

def test(jd):
  ymd = TimeOps.toYMD(jd)
  hms = TimeOps.toHMS(jd)
  datetime = str(ymd[0])+'-'+"{:0>2d}".format(ymd[1])+'-'+"{:0>2d}".format(int(math.floor(ymd[2])))+' '+"{:0>2d}".format(hms[0])+':'+"{:0>2d}".format(hms[1])
  datetime = str(datetime)
  return str("{:.1f}".format(jd)), TimeOps.formatDateTime(jd, Y_M_D_HM), datetime

print('Timestamp:\t\t\t\t\t', TimeOps.getCurrentSeconds())
print('------------------------------------------------------------')

e_date = datetime.datetime.utcnow()
print('UTC:\t\t\t\t\t\t\t\t', e_date.strftime('%Y-%m-%d %H:%M:%S'))

date = datetime.datetime.fromtimestamp(TimeOps.getCurrentSeconds());
print('Local:\t\t\t\t\t\t\t', date.strftime('%Y-%m-%d %H:%M:%S'))

print('------------------------------------------------------------')
print('Julian Date:\t\t\t\t', test((TimeOps.julianDates())) )
print('Julian Date (ephem):', test(ephem.julian_date(e_date)) )
print('Julian Date UTC:  \t', test(TimeOps.now()) )
print('Julian Date Local:\t', test(TimeOps.now(True)) )

print('------------------------------------------------------------')

print(test(2451545.0)) 
print(test(2451179.5))
print(test(2446822.5))
print(test(1355866.5))
print(test(1355671.4))
print(test(0.0))
