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
print ("-----------------------------")
for x in sys.argv:
	print (x)
print ("------------------------------")
sys.argv.insert(2,"--have-eh")
sys.argv.insert(3,"--have-std")
sys.argv.insert(4,"--fog-parse")
sys.argv.insert(5,"--root")
print ("-----------------------------")
for x in sys.argv:
	print (x)
print ("------------------------------")
sys.argv.insert(8,"--template")
sys.argv.insert(9,os.path.dirname(os.path.abspath(__file__))+"/../../Lib_UT/Runner.tpl")
print ("-----------------------------")
for x in sys.argv:
	print (x)
print ("------------------------------")
cxxtest.main(sys.argv)
print ("Done.")

