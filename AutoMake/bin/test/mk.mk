#!/bin/bash

g++ test.cc   \
    test_test_func.cc  \
-o ../../bin/test_test_func.cc


chmod a+x ../../bin/test_test_func.cc 

../../bin/test_test_func.cc

