#!/bin/bash

g++ TNet.cc   \
    test_TNet_func.cc  \
-o ../../bin/test_TNet_func.cc


chmod a+x ../../bin/test_TNet_func.cc 

../../bin/test_TNet_func.cc

