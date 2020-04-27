#!/usr/bin/env python

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
from __future__ import unicode_literals

from hypatia import *

tests = [ ]

check = True
for i in range(0, len(tests)):
  if not tests[i]:
    check = False
    print("Test number",str(i), "fail")

if not check:
  print(__file__, "FAILURE")
else:
  print(__file__, "SUCESS")
