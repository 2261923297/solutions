#!/bin/bash

g++ Hook.cc   \
    test_Hook_func.cc  \
-o ../../bin/test_Hook_func.cc


chmod a+x ../../bin/test_Hook_func.cc 

../../bin/test_Hook_func.cc

