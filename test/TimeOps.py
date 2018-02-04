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

lng = -122.0442
lat = 37.6393

obs = Observer(lng, lat)
e_obs = ephem.Observer();
e_obs.lon = MathOps.toRadians(lng)
e_obs.lat = MathOps.toRadians(lat)
e_obs.elevation = 32

sec = obs.getTime()
print('Timestamp:\t\t\t\t\t', sec)
print('------------------------------------------------------------')

e_date = datetime.datetime.utcnow()
print('UTC:\t\t\t\t\t\t\t\t', e_date.strftime('%Y-%m-%d %H:%M:%S'))

date = datetime.datetime.fromtimestamp(sec);
print('Local:\t\t\t\t\t\t\t', date.strftime('%Y-%m-%d %H:%M:%S'))

print('------------------------------------------------------------')
print('Julian Date:\t\t\t\t', obs.getJulianDate())
print('Julian Date (ephem):', ephem.julian_date(e_date))
print('Julian Date UTC:  \t', TimeOps.now())
print('Julian Date Local:\t', TimeOps.now(True))

print('------------------------------------------------------------')
# jd = obs.getJulianDate()
jd = TimeOps.now(True)
# jd = TimeOps.now()
print( TimeOps.formatDateTime( jd, Y_MON_D_HM))

# print( TimeOps.formatDateTime( 2451545.0, Y_MON_D_HM ))
# print( TimeOps.formatDateTime( 2451179.5, Y_MON_D_HM ))
# print( TimeOps.formatDateTime( 1355866.5, Y_MON_D_HM ))
# print( TimeOps.formatDateTime( 1355671.4, Y_MON_D_HM ))
# print( TimeOps.formatDateTime( 0.0, Y_MON_D_HM ))
