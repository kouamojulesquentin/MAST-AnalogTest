#! /usr/bin/env python
#
# The CxxTest driver script, which uses the cxxtest Python package.
#

import sys
import os
from os.path import realpath, dirname
if sys.version_info < (3,0):
    print ('Using Python < 3.0')
    sys.path.insert(0, dirname(dirname(realpath(__file__)))+os.sep+'python')
else:
    print ('Using Python 3.x')
    sys.path.insert(0, dirname(dirname(realpath(__file__)))+os.sep+'python'+os.sep+'python3')
sys.path.append(".")

import cxxtest

cxxtest.main(sys.argv)
print ("Done.")

