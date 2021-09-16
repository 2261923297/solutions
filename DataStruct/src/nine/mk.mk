#!/bin/bash

clear
g++ nine.cc   \
    test_nine_func.cc  \
-o ../../bin/test_nine_func.cc


chmod a+x ../../bin/test_nine_func.cc 

../../bin/test_nine_func.cc

